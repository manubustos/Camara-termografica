$(document).ready(function(){ 

  var cron;
  var sv_min = 0;
  var sv_hor = 0;
  var sv_seg = 0;

  var seg = document.getElementById('seg');
  var min = document.getElementById('min');
  var hor = document.getElementById('hor');
  
  var iniciado = false;

  function start_cron()
  {
    cron = setInterval(function()
    {
      sv_seg++;
      if(sv_seg < 60)
      {
        if(sv_seg < 10)
          seg.innerHTML = "0"+sv_seg;
        else
          seg.innerHTML = sv_seg;
      }
      else
      {
        sv_seg = 0;
        seg.innerHTML = "00";
        sv_min++;

        if(sv_min < 60)
        {
          if(sv_min < 10)
            min.innerHTML = "0"+sv_min;
          else
            min.innerHTML = sv_min;
        }
        else
        {
          sv_min = 0; min.innerHTML = "00";
          sv_hor++;

          if(sv_hor < 10)
            hor.innerHTML = "0"+sv_hor;
          else
            hor.innerHTML = sv_hor;
        }
      }
    }, 1000);
  }

  $('#btn_play').on('click', iniciar);
  function iniciar(){
    if(!iniciado)
    {
      iniciado = true;
      $.post( "escaner", function( data ){
        $( "#comando" ).html(1);
      });
      start_cron();
    }
  }

  $("#btn_pause").click(function(){
    if(iniciado)
    {
      $.post( "escaner", function( data ){
        $( "#comando" ).html(2);
      });
      clearInterval(cron);
      iniciado = false;
    }
  });

  $("#btn_stop").click(function(){
    if(iniciado)
    {
      sv_min = 0;
      sv_hor = 0;
      sv_seg = 0;

      seg.innerHTML = "00";
      min.innerHTML = "00";
      hor.innerHTML = "00";

      $.post( "escaner", function( data ){
        $( "#comando" ).html(2);
      });
      clearInterval(cron);
      iniciado = false;
    }
  });

});
