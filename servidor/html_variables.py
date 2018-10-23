# Contiene en variables las partes del html que tendra la pagina del resultado
# -*- coding: utf-8 -*-
HTML_HEAD = '''<head>
  <link rel="profile" href="http://www.w3.org/1999/xhtml/vocab">
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <!-- Nav and address bar color -->
  <meta name="theme-color" content="#774292">
  <link type="text/css" rel="stylesheet" href="../static/stylesheets/css.css" media="all">
  <link type="text/css" rel="stylesheet" href="../static/stylesheets/css_002.css" media="all">
  <link type="text/css" rel="stylesheet" href="../static/stylesheets/footer.css" media="all">
  <link type="text/css" rel="stylesheet" href="../static/stylesheets/poncho.min.css" media="all">
  <link type="text/css" rel="stylesheet" href="../static/stylesheets/custom.css" media="all">
  <link href="https://fonts.googleapis.com/css?family=Cabin" rel="stylesheet"> 
  <title>Imagen | Cámara Termográfica</title>
</head>'''

HTML_IMAGEN = '<img src="../static/images/imagen.png" alt="" />'

HTML_BODY = '''<section>
          <div class="container">
            <div class="col-xs-12 col-sm-6 col-md-4">
               <label class="labelPanel">Temperatura Mínima: &nbsp<span class="badge">70 °C</span></label>
               <br><br>
               <label class="labelPanel">Temperatura Máxima:&nbsp<span class="badge">700 °C</span></label>
               <br><br>
               <label class="labelPanel">Porcentaje Escaneado:&nbsp<span class="badge">15 %</span></label>
               <br><br>
               <label class="labelPanel">Tiempo Restante:&nbsp<span class="badge">70 seg</span></label>
               <br><br>
               <label class="labelPanel">Resolución:&nbsp<span class="badge">200 x 400</span></label>
            </div>
          </div>
        </section>'''