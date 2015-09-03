<?php
/**
 * Created by PhpStorm.
 * User: p1ratrulezzz
 * Date: 07.08.15
 * Time: 15:21
 */

if (!module_exists('masquerade')) {
  throw new Exception('Plase install and enable "masquerade" module');
}

global $user;
if (isset($_SESSION['masquerading'])) {
  $link = l(t('Switch back'), 'masquerade/unswitch', array('query' => array('token' => drupal_get_token('masquerade/unswitch'))));
  print t('You are masquerading'). "! $link";
}
