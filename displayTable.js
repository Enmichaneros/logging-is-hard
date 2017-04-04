/*
Using sql.js, access a local database file and output the contents onto an HTML table.

Requiring the user to manually choose a file is not the preferred solution, but Javascript unfortunately does not allow for automatic access of local files (for security reasons and whatnot).

Souce of sql.js: https://github.com/kripken/sql.js/
*/
$(document).ready(function() {
  var dbFileElm = document.getElementById('dbfile');

  function displayTable(file){
    // Cast file (an ArrayBuffer) to a uInt8Array (compatible with sql.js)
    var uInt8Array = new Uint8Array(file);
    // Create a database from the data and select all data
    var db = new SQL.Database(uInt8Array);

    // Creates an object with its tables as properties
    // The tables in turn are objects with two properties: an array of column names and an array of database entries
    var contents = db.exec("SELECT * FROM LOGS");

    // Loop through the column names and add to HTML
    cols = contents["0"]["columns"];
    var html = "<tr>";
    for (var i = 0; i < cols.length; i++){
      html = html + "<th>"+ cols[i] + "</th>";
    }
    html = html + "</tr>";
    $( '#display-columns' ).html( html );

    // Loop through the entries and add to HTML
    data = contents["0"]["values"];
    $ ('#display-entries').html("");
    for (var i = 0; i < data.length; i++){
      var d_html = "<tr>";
      for (var j = 0; j < data[i].length; j++){
        d_html = d_html + "<td>"+ data[i][j] + "</td>";
      }
      d_html = d_html + "</tr>";
      $( '#display-entries' ).append( d_html );
    }
  }

  // When the value of the file button changes, (user has selected a database file) read it and process
  dbFileElm.onchange = function() {
    var f = dbFileElm.files[0];
    var reader = new FileReader();
    reader.onload = function() {
      displayTable(reader.result);
    }
    reader.readAsArrayBuffer(f);
  }
});
