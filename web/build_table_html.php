<?

/**
 * Out a MySQL-style table of data
 * http://coreygilmore.com/blog/2009/02/02/print-a-mysql-style-table-with-php/
 *
 * @param	array  $data         Associative array of data to output. 
 * @param	array  $header_keys  Optional; Assoc array of display names to use for headers. Keys must match those of $data. Defaults to keys of $data.
 * @param	string $glue         String to join lines with, defaults to newline.
 * @return string
 *
 */
function build_table_html($data, $header_keys=array(), $glue="\n") {
  $table = '<table border=1 cellspacing=0 cellpadding=2>';
  $data_fmt = array();
  $divider_row = array();
  $header_fmt = array();
 
  $keys = array_keys($data[0]);
  $col_lengths = array_flip($keys); // used to determine the max column width
 
  if( empty($header_keys) ) {
    $header_keys = array_combine($keys, $keys);
  }
 
  // set the base max length to the length of our header keys
  foreach( $keys as $key ) {
    $col_lengths[$key] = strlen($header_keys[$key]);
  }
 
  foreach( $data as $row ) {
    foreach( $keys as $key ) {
      $col_lengths[$key] = max($col_lengths[$key], strlen($row[$key]));
    }
  }
 
  foreach( $keys as $key ) {
    $data_fmt[] = '%-' . $col_lengths[$key] . 's';
    $header_fmt[] = '%-' . $col_lengths[$key] . 's';
    $divider_row[] = str_pad('', $col_lengths[$key]+2, '-'); // fill the spacing
  }
  $data_fmt = '<tr><td>' . implode('</td><td>', $data_fmt) . '</td></tr>';
  #$divider_row = '+' . implode('+', $divider_row) . '+';
  $header_fmt = '<tr><th>' . implode('</th><th>', $header_fmt) . '</th></tr>';
 
  // assemble the table
  #$table .= $divider_row . $glue;
  $table .= vsprintf($header_fmt . $glue, $header_keys);
  #$table .= $divider_row . $glue;
  foreach( $data as $row ) {
    $table .= vsprintf($data_fmt . $glue, $row);
  }
  #$table .= $divider_row . $glue;
  $table .= "</table>";
 
  return $table;
}
?>
