$(document).ready(function(){ 

  var cron;
  var sv_min = 0;
  var sv_hor = 0;
  var sv_seg = 0;

  var seg = document.getElementById('seg');
  var min = document.getElementById('min');
  var hor = document.getElementById('hor');
  
  var iniciado = false;

  document.getElementById("btn_play").onclick = function() {start()};
  document.getElementById("btn_pause").onclick = function() {pause()};
  document.getElementById("btn_stop").onclick = function() {stop()};

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

  function start(){
    if(!iniciado)
    {
      iniciado = true;

      var comando = "1";
      $.post( "/enviarArduino", {"comando": comando});

      start_cron();
    }
  }

  function pause(){
    if(iniciado)
    {
      iniciado = false;

      var comando = "2";
      $.post( "/enviarArduino", {"comando": comando});

      clearInterval(cron);
    }
  }

  function stop(){
    sv_min = 0;
    sv_hor = 0;
    sv_seg = 0;

    seg.innerHTML = "00";
    min.innerHTML = "00";
    hor.innerHTML = "00";

    var comando = "3";
    $.post( "/enviarArduino", {"comando": comando});

    iniciado = false;
    clearInterval(cron);
  }

});
