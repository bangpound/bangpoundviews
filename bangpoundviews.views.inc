<?php

function bangpoundviews_views_plugins() {
  $plugins = array(
    'style' => array(
      'item_list' => array(
        'title' => t('Semantic HTML list'),
        'help' => t('Displays rows as an HTML list.'),
        'handler' => 'semanticviews_plugin_style_list',
        'theme' => 'item_list__view',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-list',
        'register theme' => FALSE,
      ),
    ),
  );

  return $plugins;
}
