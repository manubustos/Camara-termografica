$(document).ready(function(){

  $.post( "ajax/test.html", function( data ){
    $( "#comando" ).html(1);
  });

});
