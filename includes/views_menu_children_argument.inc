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

    foreach (menu_load_all() as $menu) {
      $menus[$menu['menu_name']] = $menu['title'];
    }
    asort($menus);

    unset($form['not']);
    unset($form['break_phrase']);

    $form['target_menu'] = array(
      '#type' => 'select',
      '#title' => t('Target menu'),
      '#description' => t('Select the menu to scan for child nodes.'),
      '#default_value' => $this->options['target_menu'],
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

    if (!empty($page_identifier)) {

      // If the value is an integer, we assume it is a node ID.
      if(is_numeric($page_identifier)) {
        $page_identifier = "node/$page_identifier";
      }
      self::filterByParentPage($page_identifier, $menu_name, $this->query);
    }
  }

  /**
   * @param string|integer $page_identifier The menu_item table's link_path. I.e.: node/100 (Supports an integer to default to node/%)
   * @param string $menu_name The menu's machine name we want to filter by.
   * @param \views_plugin_query $query The query we're going to alter.
   */
  public static function filterByParentPage($page_identifier, $menu_name, \views_plugin_query $query) {
    // Get the parent page menu item details.
    $parent = menu_link_get_preferred($page_identifier, $menu_name);

    $query->add_where_expression(
      0,
      'menu_links.plid = :parent_lid',
      array(':parent_lid' => $parent['mlid'])
    );
  }

  /**
   * @param \views_plugin_query $query
   * @param string $menu_name The menu's machine name we want to filter by.
   */
  public static function joinMenuLinksTableToNode(\views_plugin_query $query, $menu_name) {
    // We need to join on the menu_links table.
    // Use our special views_join object to accomplish this.
    $join = new views_menu_children_join();
    $join->prefixes[] = 'node/';
    $query->queue_table("menu_links", "node", $join);

    $query->add_where_expression(
      0,
      "menu_links.menu_name = :targetmenu"
      , array(':targetmenu' => $menu_name)
    );

  }

}
