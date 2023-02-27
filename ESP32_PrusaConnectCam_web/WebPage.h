#ifndef _WEB_PAGE_H_
#define _WEB_PAGE_H_

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
    <style>
      /* data table */
      #data {
        font-family: Arial, Helvetica, sans-serif;
        border-collapse: collapse;
        width: 100%;
        table-layout: fixed;
      }

      #data td, #data th {
        border: 1px solid #ddd;
        padding: 8px;
      }

    #data tr:nth-child(even){background-color: #f2f2f2;}
    #data tr:hover {background-color: #ddd;}

      #data th {
        padding-top: 12px;
        padding-bottom: 12px;
        text-align: left;
        background-color: #fa6831;
        color: white;
      }
  
    /* submit button */
      input[type=submit] {
        background-color: #fa6831;
        border: none;
        color: white;
        padding: 10px 32px;
        text-decoration: none;
        margin: 1px 1px;
        cursor: pointer;
      }
        
        /* text input */
      input[type=text] {
        padding: 8px 20px;
        margin: 1px 1;
        box-sizing: border-box;
        border: 3px solid #ccc;
        -webkit-transition: 0.5s;
        transition: 0.5s;
        outline: none;
        width: 210px;
      }

      input[type=text]:focus {
        border: 3px solid #555;
      }
  </style>
  <script src="http://code.jquery.com/jquery-1.12.4.js"></script>
</head>
<body>
  <div id="container">
      <center><h2>ESP32-CAM PrusaConnect cam</h2></center>
      <br><hr><h3>Buttons</hr>
      <p>
        <button onclick="capturePhoto()">CAPTURE PHOTO</button>
        <button onclick="sendPhoto();">SEND PHOTO</button>
        <button onclick="location.reload();">REFRESH PAGE</button>  
        <button onclick="led()">LED ON/OFF</button>
      </p>
      <br><hr><h3>Variables</h3>
      <table id="data">
        <tr>
            <th>Variable</th> 
            <th>Value</th>
        </tr>
        <tr>
            <td>Token</td><td id="token"></td>
        </tr>
        <tr>
            <td>Fingerprint</td><td id="fingerprint"></td>
        </tr>
        <tr>
            <td>Refresh Interval [sec]</td><td id="refreshInterval"></td>
        </tr>
        <tr>
            <td>SW version</td><td id="sw_ver"></td>
        </tr>
      </table>
    
      <br><hr><h3>Configuration</h3><br>
      <table id="input">
      <tr>
        <td>
          Token:
        </td>
        <td>
          <form action="/token">
            <input type="text" name="token">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      <tr>
        <td>
          Fingerprint:
        </td>
        <td>
          <form action="/fingerprint">
            <input type="text" name="fingerprint">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      <tr>
        <td>
          Refresh interval:
        </td>
        <td>
          <form action="/refresh">
            <input type="text" name="refresh">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      </table>
    </div>
    
    <br><hr><h3>Picture</h3>
  <div><br><center><img src="saved-photo" id="photo" width="70%"></center></div>
</body>

<script>
  function capturePhoto() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);
    xhr.send();
  }
  
  function sendPhoto() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/send", true);
    xhr.send();
  }
  
  function led() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/led", true);
    xhr.send();
  }
</script>
<script>
  (function() {
  var reqData = function() {
    jQuery.ajax({
            url: '/json_input',
            type: 'GET',
            timeout: 30000,
            success: function(data) {
              console.log("Incommming data: ");
              console.log(data);
              var obj = JSON.parse(data);
              console.log(obj);

              $("#token").text(obj.token);
              $("#fingerprint").text(obj.fingerprint);
              $("#refreshInterval").text(obj.refreshInterval);
              $("#sw_ver").text(obj.sw_ver);
            },
            error: function(html) {
              console.log("json Timeout or error");
              alert("jquery timeout or comunication error");
            }
        });
  }
  reqData();
    //var t=setTimeout(function(){reqData();},20000)
})();
</script>
</html>)rawliteral";

#endif 

/* EOF  */
