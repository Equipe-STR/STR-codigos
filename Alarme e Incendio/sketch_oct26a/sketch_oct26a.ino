#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <Arduino_JSON.h>

#define PIN_SENSOR 14
#define PIN_BUZZER 17
#define PIN_FOGO 26

const char* ssid = "STR inacio";
const char* password = "str12345";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
JSONVar readings;
int ativacaoAlarme = 1;
int ativacaoIncendio = 1;

// variáveis de tempo
unsigned long lastTime = 0;
unsigned long timerDelay = 500;

char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        *{
            font-family: Arial, Helvetica, sans-serif;
        }
        body{
            margin: 0px;
            padding: 0 px;
            background-color: rgb(241, 241, 241);
        }
        #sobre{
            display: none;
            margin: 10px;
        }
        nav{
            display: flex;
            margin: 0px;
            justify-content: space-between;
            background-color: #d5f77d;
            padding-left: 5vw;
            padding-right: 5vw;
            height: 10vh;
        }
        .list{
            display: flex;
            list-style: none;
            align-items: center;
        }
        .linksAbas{
            text-decoration: none;
            font-size: 20px;
            margin-left: 2vw;
            margin-right: 2vw;
        }
        .links{
            background-color: transparent; 
            color: black;
            font-size: 1.2rem;
            border-width: 0px;
            font-weight: bold;
        }
        .links:hover, .links::selection{
            text-decoration: underline;
            color: white;
            cursor: pointer;
        }
        #cartoes{
            padding-right: 1%;
            padding-left: 1%;
            padding-top: 3%;
            display: flex;
            justify-content: space-between;
            padding-bottom: 2em;
        }
        .cartao{
            width: 100%;
            height: 20em;
            margin-left: 3%;
            margin-right: 3%;
            background-color: white;
            border-radius: 30px;
            padding: 15px;
            display: flex;
            justify-content: center;
            flex-direction: column;
        }
        .sirene{
            width: 50px;
            height: 50px;
        }
        .cartao>*{
            display: flex;
            justify-content: center;
            align-items: center;
            margin-top:auto;
            margin-bottom: auto;
        }
        .header-cartao > *{
            display: inline;
            margin-right: 0.5em;
            margin-left: 0.5em;
        }
        .on-off-button{
            background-color: #000dfdf2; 
            color: white;
            font-size: 1.2rem;
            border-width: 0px;
            font-weight: bold;
            padding: 9px;
        }
        .situacao{
            font-size: 2em;
            font-weight: bold;
        }
        table {
            width: 90vw;
            border-collapse: collapse;
        }

        table,
        th,
        td {
            border: 1px solid black;
            padding: 15px;
            text-align: center;
            width: 90vw;
        }
        .cx-tabela {
            padding: 0 0 50px 0;
            display: flex;
            justify-content: center;
            align-items: center;
            width: 100%;
            margin: 0;
        }
        @media screen and (max-width: 522px) {
            #cartoes{
                flex-direction: column;
                align-items: center;
            }
            .cartao{
                margin-top: 10px;
                margin-bottom: 10px;
                width: 80%;
            }
        }
    </style>
    <title>Monitoramento</title>
</head>
<body>
    <nav>
        <h1>My House</h1>
        <ul class="list">
            <li class="linksAbas">
                <button class="links" id="botao-home">HOME</button>        
            </li>
            <li class="linksAbas">
                <button class="links" id="botao-sobre">SOBRE</button>
            </li>
        </ul>
    </nav>
    <div id="home">
        <div id="cartoes">
            <div class="cartao" id="alarme">
                <div class="header-cartao">
                    <h2>ALARME</h2>
                    <svg class="sirene" version="1.0" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 2048.000000 2048.000000">
                        <g transform="translate(0.000000,2048.000000) scale(0.100000,-0.100000)"
                        fill="#ff0000" stroke="none">
                        <path d="M10443 20114 c-58 -21 -126 -85 -157 -146 l-26 -52 0 -1430 c0 -1417
                        1 -1431 21 -1476 25 -55 80 -114 134 -143 32 -17 57 -22 125 -22 68 0 93 5
                        125 22 51 27 107 86 134 140 l21 42 0 1433 0 1432 -25 52 c-63 129 -220 194
                        -352 148z"/>
                        <path d="M15475 18353 c-22 -9 -49 -22 -60 -30 -11 -7 -469 -462 -1018 -1010
                        -916 -916 -999 -1002 -1017 -1050 -70 -187 67 -386 264 -385 127 1 63 -57
                        1168 1050 1114 1115 1038 1031 1038 1166 -1 104 -60 197 -157 246 -62 31 -161
                        37 -218 13z"/>
                        <path d="M5017 18129 c-136 -33 -222 -143 -221 -284 0 -55 6 -80 26 -120 20
                        -38 270 -293 1025 -1046 1107 -1105 1028 -1035 1164 -1033 192 2 321 193 256
                        379 -16 45 -109 142 -1020 1056 -784 787 -1012 1011 -1047 1026 -55 25 -133
                        35 -183 22z"/>
                        <path d="M9130 14999 c-1071 -78 -2040 -588 -2607 -1374 -208 -288 -376 -649
                        -447 -965 -13 -54 -396 -2143 -442 -2410 -8 -47 -30 -168 -49 -270 -19 -102
                        -136 -743 -260 -1425 -272 -1491 -262 -1435 -285 -1565 -31 -173 -370 -2033
                        -376 -2064 l-6 -29 5878 8 c3234 4 5880 9 5882 10 2 2 -17 99 -42 217 -25 117
                        -86 409 -136 648 -50 239 -99 476 -110 525 -10 50 -39 187 -64 305 -25 118
                        -61 289 -80 380 -20 91 -58 273 -85 405 -58 283 -265 1270 -320 1535 -22 102
                        -65 307 -96 455 -71 339 -120 574 -145 690 -11 50 -83 394 -160 765 -291 1393
                        -321 1536 -330 1575 -5 22 -21 96 -36 165 -72 342 -198 644 -392 940 -480 732
                        -1306 1253 -2257 1424 -339 61 -297 60 -1665 62 -690 1 -1307 -2 -1370 -7z"/>
                        <path d="M2543 14040 c-86 -20 -167 -96 -197 -183 -36 -106 -11 -213 67 -291
                        96 -94 -45 -87 1577 -84 l1425 3 51 27 c123 65 182 209 139 339 -22 65 -87
                        138 -153 171 l-47 23 -1415 2 c-778 0 -1429 -2 -1447 -7z"/>
                        <path d="M15580 14030 c-77 -26 -152 -106 -175 -185 -40 -137 19 -270 147
                        -334 l61 -31 1412 0 c1587 0 1473 -5 1563 76 162 145 99 416 -110 479 -45 13
                        -225 15 -1447 15 -1342 -1 -1398 -1 -1451 -20z"/>
                        <path d="M4315 4694 c-287 -51 -546 -220 -699 -456 -72 -111 -110 -193 -144
                        -316 -24 -88 -27 -112 -26 -267 0 -144 3 -183 23 -255 94 -352 353 -630 691
                        -742 169 -57 -282 -53 6385 -53 5878 0 6164 1 6243 18 404 87 711 373 819 763
                        25 90 28 114 28 269 0 155 -3 179 -28 270 -101 368 -386 649 -762 753 l-100
                        27 -6175 2 c-5319 1 -6186 -1 -6255 -13z"/>
                        <path d="M3220 2279 c-255 -28 -486 -206 -576 -446 -30 -79 -25 -94 9 -26 54
                        105 207 238 367 318 65 33 181 77 187 72 2 -2 -25 -14 -59 -27 -194 -70 -375
                        -201 -465 -336 -134 -201 -104 -421 82 -610 102 -103 227 -182 378 -239 26
                        -10 46 -20 43 -22 -8 -8 -164 60 -240 105 -92 55 -199 143 -241 198 -16 21
                        -45 66 -63 99 -35 64 -34 46 3 -48 36 -92 85 -165 157 -238 79 -79 171 -137
                        283 -176 l80 -28 4340 0 c4813 0 10036 9 10105 18 25 3 14 0 -25 -8 l-70 -13
                        163 -1 c202 -1 282 14 412 76 79 38 108 59 180 132 73 72 94 101 133 181 50
                        103 74 174 33 100 -106 -195 -359 -360 -675 -440 -46 -11 -88 -19 -93 -18 -6
                        2 25 12 68 22 44 10 126 36 183 57 314 117 516 312 550 530 49 309 -252 602
                        -739 720 -63 15 -104 28 -92 28 35 1 174 -35 272 -71 117 -43 256 -117 332
                        -175 73 -57 170 -169 195 -224 29 -67 26 -35 -5 44 -51 130 -142 245 -262 326
                        -69 48 -99 63 -180 92 l-65 24 -7285 2 c-4007 1 -7303 3 -7325 5 -22 2 -65 0
                        -95 -3z"/>
                        </g>
                    </svg>
                </div>
                <div class="situacao" id="alarme-texto">Sem detecção</div>
                <div>
                    <button id="alarme-button" class="on-off-button">LIGADO</button>
                </div>
                
            </div>
            <div class="cartao" id="incendio">
                <div class="header-cartao">
                    <h2>INCÊNDIO</h2>
                    <svg class="sirene" xmlns="http://www.w3.org/2000/svg" version="1.0" width="895.000000pt" height="1280.000000pt" viewBox="0 0 895.000000 1280.000000" preserveAspectRatio="xMidYMid meet">
                        <metadata>
                        Created by potrace 1.15, written by Peter Selinger 2001-2017
                        </metadata>
                        <g transform="translate(0.000000,1280.000000) scale(0.100000,-0.100000)" fill="#ff0000" stroke="none">
                        <path d="M4140 12724 c-209 -242 -356 -536 -419 -841 -62 -302 -64 -619 -5 -1003 28 -182 56 -317 139 -677 79 -344 98 -465 93 -593 -5 -119 -24 -171 -80 -213 -65 -49 -64 -49 -166 92 -164 226 -394 474 -587 631 -232 188
                        -476 313 -695 354 -79 15 -310 22 -323 10 -3 -3 18 -38 47 -78 29 -39 96 -136 149 -216 341 -520 543 -1027 609 -1530 16 -126 16 -461 -1 -580 -17 -123 -71 -333 -111 -435 -36 -90 -89 -195 -100 -195 -3 0 -31 30 -61 67 -85
                        105 -295 310 -425 415 -255 206 -531 362 -807 457 -255 88 -507 108 -507 40 0 -11 34 -45 83 -83 271 -212 464 -515 587 -926 101 -336 147 -738 137 -1190 -5 -207 -29 -522 -42 -558 -3 -8 -35 11 -92 56 -336 264 -722 465 -993
                        516 -94 18 -232 21 -311 6 -62 -12 -178 -67 -228 -108 l-34 -29 97 -52 c53 -29 143 -83 199 -120 492 -325 680 -680 645 -1218 -9 -144 -22 -242 -73 -557 -20 -120 -42 -274 -50 -341 -62 -534 5 -1173 177 -1680 220 -650 620
                        -1217 1130 -1600 l99 -75 -296 0 -295 0 0 -235 0 -235 2855 0 2855 0 0 235 0 235 -342 0 -343 0 155 78 c1134 574 1881 1547 2085 2717 45 265 50 320 49 605 -2 692 -125 1265 -335 1559 -24 33 -40 48 -43 40 -2 -8 -14 -45 -26
                        -84 -63 -200 -201 -365 -342 -407 -51 -15 -58 -5 -58 82 0 84 16 167 91 463 135 537 159 643 180 811 22 181 -3 454 -62 662 -76 273 -216 515 -412 714 -78 79 -264 228 -272 218 -1 -2 1 -34 6 -73 12 -101 11 -456 -1 -555 -25
                        -201 -82 -416 -147 -544 -42 -85 -102 -167 -172 -239 -56 -58 -145 -127 -163 -127 -16 0 -21 96 -28 625 -10 707 -25 935 -86 1300 -155 930 -532 1546 -1129 1843 -90 45 -233 102 -255 102 -4 0 22 -66 60 -147 186 -406 286
                        -734 315 -1034 8 -80 -47 -580 -62 -565 -2 2 -15 59 -29 127 -107 528 -464 1184 -954 1754 -162 189 -253 344 -364 618 -169 421 -318 998 -411 1597 -15 96 -30 178 -34 182 -3 4 -35 -27 -71 -68z m1400 -7296 c62 -145 85 -383
                        77 -817 -6 -338 -27 -644 -72 -1036 -14 -121 -28 -246 -32 -278 l-6 -58 26 10 c241 92 511 363 905 911 61 85 125 171 141 190 29 33 32 34 45 17 34 -43 32 -224 -5 -457 -70 -446 -244 -869 -450 -1095 l-53 -58 34 8 c125 26 309
                        109 448 202 96 65 245 202 324 299 28 35 53 63 54 61 5 -5 -36 -193 -66 -300 -250 -896 -888 -1647 -1765 -2079 -169 -83 -397 -176 -555 -226 -344 -109 -326 -105 -424 -100 -212 12 -524 162 -803 386 -494 398 -805 942 -874
                        1526 -7 62 -10 175 -6 300 7 281 48 490 154 776 35 98 192 430 202 430 3 0 32 -105 63 -233 217 -879 540 -1459 923 -1657 65 -34 149 -59 161 -47 3 2 -13 69 -35 148 -216 792 -277 1479 -166 1864 28 95 51 146 98 217 33 50
                        47 59 47 33 0 -29 92 -260 155 -390 159 -327 357 -535 561 -589 l51 -13 22 101 c172 786 428 1463 727 1924 29 45 58 82 63 82 4 0 19 -24 31 -52z"/>
                        </g>
                    </svg>
                </div>
                <div class="situacao" id="incendio-texto">Sem detecção</div>
                <div>
                    <button id="incendio-button" class="on-off-button">LIGADO</button>
                </div>
            </div>
        </div>
        <div class="cx-tabela">
            <table id="historico">
                <tr>
                    <th colspan="3" style="background-color: #d5f77d;">Histórico</th>
                </tr>
                <tr>
                    <th>Hora</th>
                    <th>Evento</th>
                    <th>Data</th>
                </tr>
            </table>
        </div>
    </div>
    <div id="sobre">
        <h2>SOBRE</h2>
        <p>Site feito na disciplina de Software em Tempo Real (STR) usando um ESP32 e Web Socket para a comunicação cliente-servidor</p>
    </div>
</body>
<Script>
    const botaoIncedio = document.querySelector("#incendio-button");
    botaoIncedio.addEventListener("click", function (e){
        const botaoIncedio = document.querySelector("#incendio-button");
        e.preventDefault();
        if(botaoIncedio.innerHTML == "DESLIGADO"){
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/ativarincendio", true);
            xhttp.send();
        }
        else{
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/desativarincendio", true);
            xhttp.send()
        }
     })
    const botaoAlarme = document.querySelector("#alarme-button");
    botaoAlarme.addEventListener("click", function (e){
        const botaoAlarme = document.querySelector("#alarme-button");
        e.preventDefault();
        if(botaoAlarme.innerHTML == "DESLIGADO"){
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/ativaralarme", true);
            xhttp.send()
        }
        else{
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/desativaralarme", true);
            xhttp.send()
        }
     })

    function n(){}
    function clickHome(e) {
        e.preventDefault();
        document.getElementById("home").style.display = "block";
        document.getElementById("sobre").style.display = "none";
    }
    function clickSobre(e) {
        e.preventDefault();
        document.getElementById("home").style.display = "none";
        document.getElementById("sobre").style.display = "block";
    }
    document.getElementById("botao-home").addEventListener("click", clickHome);
    document.getElementById("botao-sobre").addEventListener("click", clickSobre);
    
    function addCard(nomeEvento) {
        // Seleciona a tabela
        var table = document.querySelector('#historico');
        // Insere uma nova linha no final da tabela
        var newRow = table.insertRow(2);
        // Insere novas células na linha
        var hora = newRow.insertCell(0);
        var evento = newRow.insertCell(1);
        var data = newRow.insertCell(2);
        var now = new Date();
        // Adiciona o conteúdo às células
        hora.innerHTML = now.toLocaleTimeString();
        evento.innerHTML = nomeEvento;
        data.innerHTML =  now.toLocaleDateString();
    }
    
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    // Init web socket when the page loads
    window.addEventListener('load', onload);
    var alarme = false;
    var incendio = false;
    function onload(event) {
        initWebSocket();
    }

    function getReadings(){
        websocket.send("getReadings");
    }

    function initWebSocket() {
        console.log('Tentando abrir conecção com o Web Socket');
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
    }

    // When websocket is established, call the getReadings() function
    function onOpen(event) {
        console.log('Connection opened');
        getReadings();
    }

    function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 2000);
    }

    function ativarAlarme(){
        addCard("Alarme iniciado")
        const elemento = document.getElementById("alarme-texto")
        elemento.style.color = "#ff0000"
        elemento.innerHTML = "ALARME ATIVADO"
    }
    
    function ativarIncendio(){
        const elemento = document.getElementById("incendio-texto")
        elemento.style.color = "#ff0000"
        elemento.innerHTML = "INCÊNDIO DETECTADO"
        addCard("Incendio iniciado")
    }

    function desativarAlarme(){
        const elemento = document.getElementById("alarme-texto")
        elemento.style.color = "#000000"
        elemento.innerHTML = "Sem detecção"
        addCard("Alarme terminado")
    }
    
    function desativarIncendio(){
        const elemento = document.getElementById("incendio-texto")
        elemento.style.color = "#000000"
        elemento.innerHTML = "Sem detecção"
        addCard("Incendio terminado")
    }

    function onMessage(event) {
        console.log(event.data);
        var myObj = JSON.parse(event.data);
        var keys = Object.keys(myObj);
        if (myObj["sinalPresenca"]=="1" && !alarme){
            alarme = true
            ativarAlarme()
        }
        if (myObj["leituraFogo"]=="1"&& !incendio){
            incendio = true
            ativarIncendio()
        }
        if (myObj["sinalPresenca"]=="0" && alarme){
            alarme = false
            desativarAlarme()
        }
        if (myObj["leituraFogo"]=="0" && incendio){
            incendio = false
            desativarIncendio()
        }
        if (myObj["ativacaoAlarme"]=="0"){
            const botaoAlarme = document.querySelector("#alarme-button");
            botaoAlarme.innerHTML = "DESLIGADO";
            botaoAlarme.style.backgroundColor = "#FD002E";
        }
        if (myObj["ativacaoAlarme"]=="1"){
            const botaoAlarme = document.querySelector("#alarme-button");
            botaoAlarme.innerHTML = "LIGADO";
            botaoAlarme.style.backgroundColor = "#000dfdf2";
        }
        if (myObj["ativacaoIncendio"]=="0"){
            const botaoIncendio = document.querySelector("#incendio-button");
            botaoIncedio.innerHTML = "DESLIGADO";
            botaoIncedio.style.backgroundColor = "#FD002E";
        }
        if (myObj["ativacaoIncendio"]=="1"){
            const botaoIncendio = document.querySelector("#incendio-button");
            botaoIncendio.innerHTML = "LIGADO";
            botaoIncendio.style.backgroundColor = "#000dfdf2";
        }
    }
</script>
</html>
)rawliteral";

void removeLineInfo(char *input) {
    char *lineStart = strstr(input, "#line");
    
    while (lineStart != NULL) {
        char *lineEnd = strchr(lineStart, '\n');
        
        if (lineEnd != NULL) {
            char *secondLineEnd = strchr(lineEnd + 1, '\n');
            if (secondLineEnd != NULL) {
                // Move tudo após a segunda quebra de linha para o início da string
                memmove(lineStart, secondLineEnd + 1, strlen(secondLineEnd + 1) + 1);
            } else {
                // Se não houver segundo '\n', apenas remova a linha '#line'
                lineStart[0] = '\0';
            }
        } else {
            // Se não houver quebra de linha, apenas remova a linha "#line"
            lineStart[0] = '\0';
        }
        
        lineStart = strstr(input, "#line");
    }
}

void configureIOPin(){
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_FOGO, INPUT);
}

void acionaBuzzer(){
  tone(PIN_BUZZER, 1000);
  delay(300);
  tone(PIN_BUZZER, 500);
  delay(200);
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

String getSensorReadings(){
  int sinalPresenca;
  bool leituraFogo;
  if (ativacaoAlarme){
    sinalPresenca = digitalRead(PIN_SENSOR);
  }
  else{
    sinalPresenca = 0;
  }
  if (ativacaoIncendio){
    leituraFogo = digitalRead(PIN_FOGO);
  }else{
    leituraFogo = LOW;
  }
  if(sinalPresenca == HIGH){
    acionaBuzzer();
  }
  else if(leituraFogo==HIGH){
    acionaBuzzer();
  }
  //LOW: Nada detectado
  else{
    // Desativa o buzzer
    noTone(PIN_BUZZER);
  }
  readings["sinalPresenca"] = String(sinalPresenca);
  readings["leituraFogo"] =  String(leituraFogo);
  readings["ativacaoAlarme"] = String(ativacaoAlarme);
  readings["ativacaoIncendio"] = String(ativacaoIncendio);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char*)data;
    //Check if the message is "getReadings"
    if (strcmp((char*)data, "getReadings") == 0) {
      // if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      Serial.print(sensorReadings);
      notifyClients(sensorReadings);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}


void configurarWifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wifi....");
  }
  Serial.print("IP para conectar: ");
  Serial.println(WiFi.localIP());
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void configurarRotas(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/desativaralarme", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Alarme desativado");
    ativacaoAlarme = false;
    request->send_P(200, "text/html", "Desativado com sucesso");
  });
  server.on("/ativaralarme", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Alarme ativado");
    ativacaoAlarme = true;
    request->send_P(200, "text/html", "Ativado com sucesso");
  });
  server.on("/desativarincendio", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Incendio desativado");
    ativacaoIncendio = false;
    request->send_P(200, "text/html", "Desativado com sucesso");
  });
  server.on("/ativarincendio", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Incendio ativado");
    ativacaoIncendio = true;
    request->send_P(200, "text/html", "Ativado com sucesso");
  });
}

void setup() {
  initWebSocket();
  removeLineInfo(index_html);
  // Corrigir linhas extras adicionadas ao html
  ledcSetup(1, 2, 3);
  // Para corrigir um bug
  configureIOPin();

  Serial.begin(9600);

  configurarWifi();

  configurarRotas();
  server.begin();
}

void loop() {
  String ultimoResultado = " ";
  while (1){
    if ((millis() - lastTime) > timerDelay) {
      String sensorReadings = getSensorReadings();
      if (ultimoResultado == " "){
        Serial.println(sensorReadings);
        Serial.println("Primeira execucao");
        ultimoResultado = sensorReadings;
      }
      if (ultimoResultado != sensorReadings){
        ultimoResultado = sensorReadings;
        Serial.println(sensorReadings);
        notifyClients(sensorReadings);
      }
      lastTime = millis();
      Serial.println("LENDO");
    }
  }
}