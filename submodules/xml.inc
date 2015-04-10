<?php
/**
 * @file
 *
 * Functions for working with XML
 */

/**
 * Deliver callback.
 */
function gb_common_xml_deliver($content) {
  // Return default error page.
  if (is_int($content)) {
    drupal_deliver_html_page($content);
  }

  $content = gb_common_xml_encode($content);
  drupal_add_http_header('Content-Type', 'application/xml; charset=utf-8');
  drupal_add_http_header('Content-Length', strlen($content));
  echo $content;
  drupal_exit();
}

/**
 * Php variable to xml converter.
 */
function gb_common_xml_encode($var, $tag = NULL, $depth = 0) {
  $_myself = __FUNCTION__;
  $content = '';

  if ($depth === 0) {
    $content .= '<?xml version="1.0"?>' . "\n" . $content;
  }

  if (is_array($var) && !empty($var)) {
    $tag = isset($var['#tag']) ? $var['#tag'] : $tag;
    $inner_content = '';
    foreach ($var as $key => $value) {
      if ($key === '#tag') {
        continue;
      }

      $inner_content .= "\n";
      $inner_content .= $_myself($value, $key, $depth + 1);
    }

    if (!empty($inner_content)) {
      $inner_content .= "\n";

      for ($i = 0; $i < $depth; $i++) {
        $inner_content .= '  ';
      }
    }

    for ($i = 0; $i < $depth; $i++) {
      $content .= '  ';
    }

    $content .= trim($inner_content) !== '' ? ('<' . $tag . '>' . $inner_content . '</' . $tag . '>') : ('<' . $tag . '/>');
  }
  else {
    for ($i = 0; $i < $depth; $i++) {
      $content .= '  ';
    }

    $content .= trim($var) !== '' ? ('<' . $tag . '>' . $var . '</' . $tag . '>') : ('<' . $tag . '/>');
  }

  return $content;
}
