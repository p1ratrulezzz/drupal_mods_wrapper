<?php
// $Id$

/**
 * @file better_exposed_filters.views.inc
 *
 * Adds Views3 support
 *
 * Views3 adds the concept of exposed forms to the mix.  In addition, elements injected into a Views
 * dialog is no longer saved along with the Views form information (see the unpack_options() and
 * options_definition() methods of the views_object object).
 *
 */

/**
 * Implements hook_views_plugins().
 */
function gb_common_views_plugins() {
  return gb_common_call_func('main_core', 'hook_views_plugins');
}
