<?php

/**
 * Implements hook_drush_command().
 */
function getprofiles_drush_command() {
  $items = array();

  $items['get-install-profiles'] = array(
    'description' => dt('Get drupal site install profiles.'),
    'aliases' => array('gip'),
    'arguments' => array(
      'all' => dt('List all drupal install profiles.'),
      'current' => dt('Print out current drupal install profile.'),
    ),
    'options' => array(
      'description' => dt('Outputs install profile description.'),
    ),
    'callback' => 'drush_getprofiles_get_install_profiles',
  );

  return $items;
}

/**
 * Output the line that matches the pattern from subject.
 *
 * @param $pattern: The pattern to search for.
 * @param $replacement: A string to replace pattern.
 * @param $subject: Array with string to search and replace.
 */
function search_line($pattern, $replacement, $subject) {
  for ($i = 0; $i < count($subject); $i++){
    if (preg_match('#' . $pattern . '#', $subject[$i])){
      print(preg_replace('#' . $pattern . '#', $replacement, $subject[$i]));
      break;
    }
  }
}

/**
 * Command callback for drush get-install-profiles.
 */
function drush_getprofiles_get_install_profiles($arg = NULL) {
  $opt = drush_get_option('description');
  $files = file_scan_directory('./profiles', '/\.profile/');
  $correct_args = array('all', 'current');
  if (!in_array($arg, $correct_args)) {
    return drush_set_error('INVALID_ARGUMENT', dt('"@arg" is not a valid argument. Enter \'drush help gip\' for help.', array('@arg' => $arg)));
  }

  switch ($arg) {
    case 'current':
      print("Listing out current install profile...\n");
      $current_profile = drupal_get_profile();
      print(dt("!profile is the current install profile.\n", array('!profile' => $current_profile)));
      if ($opt) {
        foreach ($files as $profile) {
          if ($profile->name == $current_profile) {
            $profile_info = file(str_replace('.profile', '.info', $profile->uri));
            search_line('description = ', 'Description: ', $profile_info);
          }
        }
      }
      break;

    case 'all':
      print("Listing out all install profiles...\n");
      if ($opt) {
        foreach ($files as $profile) {
          $profile_info = file(str_replace('.profile', '.info', $profile->uri));
          search_line('name =', 'Name:', $profile_info);
          search_line('description = ', 'Description: ', $profile_info);
          echo "\n";
        }
      }
      else {
        foreach ($files as $profile) {
          print('Name: ' . $profile->name . "\n");
        }
      }
      break;
  }
}