<?php
/**
 * @file
 * Contains the default style plugin.
 */

/**
 * Default style plugin to render rows one after another with no
 * decorations.
 *
 * @ingroup views_style_plugins
 */
class semanticviews_plugin_style_list extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'ul');
    $options['class'] = array('default' => '');
    $options['wrapper_class'] = array('default' => 'item-list');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('List type'),
      '#options' => array('ul' => t('Unordered list'), 'ol' => t('Ordered list')),
      '#default_value' => $this->options['type'],
    );
    $form['wrapper_class'] = array(
      '#title' => t('Wrapper class'),
      '#description' => t('The class to provide on the wrapper, outside the list.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['wrapper_class'],
      '#disabled' => TRUE,
    );
    $form['class'] = array(
      '#title' => t('List class'),
      '#description' => t('The class to provide on the list element itself.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['class'],
    );
  }

  function render_grouping_sets($sets, $level = 0) {
    $output = '';
    foreach ($sets as $set) {
      $row = reset($set['rows']);
      // Render as a grouping set.
      if (is_array($row) && isset($row['group'])) {
        $output .= theme(views_theme_functions('views_view_grouping', $this->view, $this->display),
          array(
            'view' => $this->view,
            'grouping' => $this->options['grouping'][$level],
            'grouping_level' => $level,
            'rows' => $set['rows'],
            'title' => $set['group'])
        );
      }
      // Render as a record set.
      else {

        if ($this->uses_row_plugin()) {
          foreach ($set['rows'] as $index => $row) {
            $this->view->row_index = $index;
            $set['rows'][$index] = $this->row_plugin->render($row);
          }
        }

        $items = array();
        foreach ($set['rows'] as $index => $row) {
          $item = $this->view->row_attributes[$index];
          if ($row_class = $this->get_row_class($id)) {
            $item['class'][] = $row_class;
          }
          $item['data'] = $row;
          $items[] = $item;
        }

        $variables = array(
          'items' => $items,
          'type' => $this->options['type'],
          'view' => $this->view,
          'options' => $this->options,
          'grouping_level' => $level,
          'rows' => $set['rows'],
        );
        if (!empty($set['group'])) {
          $variables['title'] = $set['group'];
        }
        if (!empty($this->options['class'])) {
          $variables['attributes'] = array('class' => $this->options['class']);
        }


        $output .= theme($this->theme_functions(), $variables);
      }
    }
    unset($this->view->row_index);
    return $output;
  }
}
