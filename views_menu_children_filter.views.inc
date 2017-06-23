<?php

/**
 * Implements hook_views_data_alter().
 */
function views_menu_children_filter_views_data_alter(&$data) {
  $data['node']['menu_children_filter'] = array(
    'title' => t('Menu children'),
    'help' => t('Gets one level of child content for argument Content ID within the target menu.'),
    'argument' => array(
      'handler' => 'views_menu_children_argument'
    ),
  );
  $data['node']['menu_children_sort'] = array(
    'title' => t('Menu children'),
    'help' => t('Sorts by the content\'s weight in the selected menu.'),
    'sort' => array(
      'handler' => 'views_menu_children_sort_handler'
    ),
  );
}
