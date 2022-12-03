setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 10000 ) ;

  var smk = ("%SMOKE%");

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        smk = this.responseText;
      }
    };
    xhttp.open("GET", "/smoke", true);
    xhttp.send();
    if (smk== 1){
      document.getElementById("smoke").innerHTML = ("Detected !");
    }
    else if (smk==0){
      document.getElementById("smoke").innerHTML = ("Normal");
    }
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("btemperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/btemperature", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("bpm").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/bpm", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("lng").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/lng", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("lat").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/lat", true);
    xhttp.send();
  }, 10000 ) ;

  var soos = ("%SOS%");


  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        soos = this.responseText;
      }
    };
    xhttp.open("GET", "/sos", true);
    xhttp.send();
    if (soos == 2){
      gosos();
    }
  }, 5000 ) ;

    function gosos(){
      location.replace("sos.html");
    } 

    

   