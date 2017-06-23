# Views Menu Children Filter

This module adds a contextual filter to Views that will retrieve a menu item's 
"child" content. Filtering by page path to accommodate parent pages by path 
rather than Content ID (ie. a Views page or Page Manager page) is possible 
thanks to @badrange.

This allows you to (for example) create a view of pages that are direct children
of a specific page in the Primary navigation.

## Installation and Basic Usage

 - Install this module using the official 
  [Backdrop CMS instructions](https://backdropcms.org/guide/modules)
 - Create a view that selects `Content`.
 - Add the "Menu children" contextual filter. Specify the menu that the filter 
 should target (required field).
 - Provide a Content ID or path to a parent page that you want to display the 
 child content from.

## Issues

Bugs and Feature requests should be reported in the 
[Issue Queue](https://github.com/backdrop-contrib/views_menu_children_filter/issues)

## Current Maintainers

 - [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org)
 - Help welcome!

## Credits

- Ported to Backdrop CMS by [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org).
- Maintainers for Drupal: [Brian Young/cravecode](https://www.drupal.org/u/cravecode) 
and [Ben McClure/bmcclure](https://www.drupal.org/u/bmcclure).

## License

This project is GPL v2 software. See the [LICENSE.txt](https://github.com/backdrop-contrib/views_menu_children_filter/blob/1.x-1.x/LICENSE.txt) 
file in this directory for complete text.