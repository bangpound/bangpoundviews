<?php
/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item in an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class semanticviews_plugin_style_list extends views_plugin_style_list {

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['wrapper_class']['#disabled'] = TRUE;
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
          if ($row_class = $this->get_row_class($index)) {
            $item['class'][] = views_clean_css_identifier($row_class);
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
          $class = explode(' ', $this->options['class']);
          $class = array_map('views_clean_css_identifier', $class);
          $variables['attributes'] = array('class' => $class);
        }

        $output .= theme($this->theme_functions(), $variables);
      }
    }
    unset($this->view->row_index);
    return $output;
  }
}
