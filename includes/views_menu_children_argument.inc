<?php

class views_menu_children_argument extends views_handler_argument_numeric {

  /**
   * The operator used for the query: or|and.
   * @var string
   */
  var $operator;

  /**
   * The actual value which is used for querying.
   * @var array
   */
  var $value;

  function option_definition() {
    $options = parent::option_definition();

    $options['target_menu'] = array(
      'default' => ''
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['default_action']["#options"]['root_nodes'] = t('Display root nodes only.');

    unset($form['not']);
    unset($form['break_phrase']);

    self::buildMenuSelectForm($form, $form_state, $this->options['target_menu']);
  }

  public static function buildMenuSelectForm(&$form, &$form_state, $default_menu) {
    foreach (menu_load_all() as $menu) {
      $menus[$menu['menu_name']] = $menu['title'];
    }
    asort($menus);

    $form['target_menu'] = array(
      '#type' => 'select',
      '#title' => t('Target menu'),
      '#description' => t('Select the menu to scan for child nodes.'),
      '#default_value' => $default_menu,
      '#required' => TRUE,
      '#options' => array_merge(array('' => t('-- Select menu --')), $menus),
    );

  }

  function title() {
    return parent::title();
  }

  /**
   * Override for specific title lookups.
   * @return array
   *    Returns all titles, if it's just one title it's an array with one entry.
   */
  function title_query() {
    return $this->value;
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $menu_name = $this->options['target_menu'];

    // The magic sauce! Joining the menu_links table onto the node table ;)
    self::joinMenuLinksTableToNode($this->query, $menu_name);

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $page_identifier = reset($this->value);

    // If the value is an integer, we assume it is a node ID.
    if(is_numeric($page_identifier) && $page_identifier != "0") {
      $page_identifier = "node/$page_identifier";
    }
    self::filterByParentPage($page_identifier, $menu_name, $this->query);
  }

  /**
   * Filter the query by either a: parent node, page page via its link_path, or null and limit to root nodes.
   *
   * @param null|string|integer $page_identifier The menu_item table's link_path. I.e.: node/100 (Supports an integer to default to node/%)
   * @param string $menu_name The menu's machine name we want to filter by.
   * @param \views_plugin_query $query The query we're going to alter.
   */
  public static function filterByParentPage($page_identifier, $menu_name, \views_plugin_query $query) {
    // If not page identifier is provided, select only root nodes of the menu.
    if(empty($page_identifier)) {
      $parent['mlid'] = 0;
    }
    else {
      // Get the parent page menu item details.
      $parent = menu_link_get_preferred($page_identifier, $menu_name);
    }

    $query->add_where_expression(
      0,
      'menu_links.plid = :parent_lid',
      array(':parent_lid' => $parent['mlid'])
    );
  }

  /**
   * @param \views_plugin_query $query
   * @param string $menu_name The menu's machine name we want to filter by.
   * @param bool $allow_duplicate_join If "false", prevents this join from joining more than once if this function is called repeatedly.
   */
  public static function joinMenuLinksTableToNode(\views_plugin_query $query, $menu_name, $allow_duplicate_join = false) {
    // Because this can be called from the argument and sort handlers,
    // first check to see if the join as already been applied.
    if(!$allow_duplicate_join && isset($query->tables['node']['menu_links'])) {
      return;
    }

    // We need to join on the menu_links table.
    // Use our special views_join object to accomplish this.
    $join = new views_menu_children_join();
    $join->prefixes[] = 'node/';
    drupal_alter('views_menu_children_filter_join', $join->prefixes, $menu_name, $query);
    $query->queue_table("menu_links", "node", $join);

    $query->add_where_expression(
      0,
      "menu_links.menu_name = :targetmenu"
      , array(':targetmenu' => $menu_name)
    );

  }

  /**
   * Determine if the argument is set to provide a default argument.
   */
  function has_default_argument() {
    return ($this->options['default_action'] == 'root_nodes'
      ? true
      : parent::has_default_argument());
  }

  function get_default_argument() {
    return "0";
  }
}
