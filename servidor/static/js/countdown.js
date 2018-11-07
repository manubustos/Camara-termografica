// Cuenta el tiempo de escaneo y calcula el porcentaje restante 
// Set the date we're counting down to
var tiempo = 6; // Tiempo que tarda en procesarse todos los datos para que se habilite el boton VerResultado
var countDownDate = new Date().getTime();
countDownDate += 1000*60*tiempo;
contador = false;
// Update the count down every 1 second
var x = setInterval(function() {
    // Get todays date and time
    var now = new Date().getTime();
    // Find the distance between now an the count down date
    var distance = countDownDate - now;
    var ms = tiempo*60000/1 // Pasa de minutos a ms para calcular el porcentaje
    var porcentaje = (100 - (distance * 100 / ms)).toFixed(2);;    // toFixed se queda con dos digitos despues de la coma   
    // Time calculations for minutes and seconds
    var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
    var seconds = Math.floor((distance % (1000 * 60)) / 1000);
    // If the count down is over, write some text 
    if (distance <= 0) {
      contador = false;
      window.location.reload(1);
    }
    // Output the result in an element with id="demo"
    if (contador == true) {
        document.getElementById("demo").innerHTML =  minutes + "m " + seconds + "s ";
        document.getElementById("porcentaje").innerHTML =  porcentaje + "%";
    }
}, 1000); // Cada cuánto tiempo cuenta

function start_contador(){
  contador = true;
  countDownDate = new Date().getTime();
  countDownDate += 1000*60*tiempo; 
}
