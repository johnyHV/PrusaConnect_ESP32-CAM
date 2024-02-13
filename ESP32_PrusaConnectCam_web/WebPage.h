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
        margin: 1px 1px;
        box-sizing: border-box;
        border: 3px solid #ccc;
        -webkit-transition: 0.5s;
        transition: 0.5s;
        outline: none;
        width: 210px;
        text-align: center;
      }

      input[type=text]:focus {
        border: 3px solid #555;
      }

      .button {
        background-color: #fa6831;
        border: none;
        color: white;
        padding: 10px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        margin: 1px 1px;
        cursor: pointer;
      }
      
      .select {
        /*background-color: #fa6831;*/
        border: 3px solid #ccc;
        /*color: white;*/
        padding: 8px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        margin: 1px 1px;
        cursor: pointer;
        width: 210px;
      }
  </style>
  <script src="http://code.jquery.com/jquery-1.12.4.js"></script>
</head>
<body>
  <div id="container">
      <center><h2>ESP32-CAM PrusaConnect cam</h2></center>
      <br><hr><h3>Buttons</hr>
      <p>
        <button class="button" onclick="capturePhoto()">CAPTURE PHOTO</button>
        <button class="button" onclick="sendPhoto();">SEND PHOTO</button>
        <button class="button" onclick="location.reload();">REFRESH PAGE</button>  
        <button class="button" onclick="led()">LED ON/OFF</button>
        <br><br>
        <button class="button" onclick="reset()">Reset to default configuration</button>
        <button class="button" onclick="reboot()">Reboot MCU</button>
        <label><input type="checkbox" id="autoPhoto" onclick="automaticPhoto(this);" checked>
    automatic photo</label>
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
            <td>Photo Quality</td><td id="photoquality"></td>
        </tr>
        <tr>
            <td>Framesize</td><td id="framesize"></td>
        </tr>
        <tr>
            <td>Brightness</td><td id="brightness"></td>
        </tr>
        <tr>
            <td>Contrast</td><td id="contrast"></td>
        </tr>
        <tr>
            <td>Saturation</td><td id="saturation"></td>
        </tr>
        <tr>
            <td>Hmirror</td><td id="hmirror"></td>
        </tr>
        <tr>
            <td>Vflip</td><td id="vflip"></td>
        </tr>
        <tr>
            <td>Lens correction</td><td id="lensc"></td>
        </tr>
        <tr>
            <td>Exposure_ctrl</td><td id="exposure_ctrl"></td>
        </tr>
        <tr>
            <td>Led Status</td><td id="led"></td>
        </tr>
        <tr>
            <td>Signal Strength (RSSI) [dBm]</td><td id="rssi"></td>
        </tr>
        <tr>
            <td>Camera flash</td><td id="camera_flash"></td>
        </tr>
        <tr>
            <td>Camera flash duration [ms]</td><td id="camera_flash_duration"></td>
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
            <input type="text" name="token" value="Your token">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Refresh interval [sec]:
        </td>
        <td>
          <form action="/refresh">
            <input type="text" name="refresh" value="10">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Photo quality [10-63]:
        </td>
        <td>
          <form action="/photo_quality">
            <input type="text" name="photo_quality" value="10">
            <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Framesize:
        </td>
        <td>
          <form action="/framesize">
          <label for="framesize"></label>
          <select class="select" id="framesize" name="framesize">
            <option value="0">QVGA (320x240)</option>
              <option value="1">CIF (352x288)</option>
              <option value="2">VGA (640x480)</option>
              <option value="3">SVGA (800x600)</option>
              <option value="4">XGA (1024x768)</option>
              <option selected value="5">SXGA (1280x1024)</option>
              <option value="6">UXGA (1600x1200)</option>
          </select>
          <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Brightness:
        </td>
        <td>
          <form action="/brightness">
          <label for="brightness"></label>
          <select class="select" id="brightness" name="brightness">
            <option value="-2">-2</option>
              <option value="-1">-1</option>
              <option selected value="0">0</option>
              <option value="1">1</option>
              <option value="2">2</option>
          </select>
          <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Contrast:
        </td>
        <td>
          <form action="/contrast">
          <label for="contrast"></label>
          <select class="select" id="contrast" name="contrast">
            <option value="-2">-2</option>
              <option value="-1">-1</option>
              <option selected value="0">0</option>
              <option value="1">1</option>
              <option value="2">2</option>
          </select>
          <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      
      <tr>
        <td>
          Saturation:
        </td>
        <td>
          <form action="/saturation">
          <label for="saturation"></label>
          <select class="select" id="saturation" name="saturation">
            <option value="-2">-2</option>
              <option value="-1">-1</option>
              <option selected value="0">0</option>
              <option value="1">1</option>  
              <option value="2">2</option>
          </select>
          <input type="submit" value="Save">
          </form>
        </td>
      </tr>
      </table><br>
      
      <h4>Photo configuration</h4>
      <form action="/action_page">
      <input type="checkbox" name="hmirror" value="1">
      <label for="hmirror"> Hmirror &emsp;&emsp;&emsp; - horizontal mirror</label><br>
        
      <input type="checkbox" name="vflip" value="1">
      <label for="vflip"> Vflip &emsp;&emsp;&emsp;&emsp;&nbsp; - vertical flip</label><br>
        
      <input type="checkbox" name="lenc" value="1" checked>
      <label for="lenc"> lenc &emsp;&emsp;&emsp;&emsp;&ensp;&nbsp; - lens correction</label><br>
        
      <input type="checkbox" name="exposure_ctrl" value="1" checked>
      <label for="exposure_ctrl"> exposure_ctrl &emsp; - Set exposure control</label><br>
        
      <input type="submit" value="Save">
      </form>
      
      <br>
      <h4>Camera Flash function</h4>
      <table>
      <tr><td>
        Camera Flash function
      </td><td>
      <form action="/camera_flash">
      <input type="submit" value="Enable/Disable">
    </form>
      </td>
      <tr>
      <td>
          Camera flash duration [ms]
        </td>
        <td>
          <form action="/camera_flash_duration">
            <input type="text" name="flashduration" value="200">
            <input type="submit" value="Save">
          </form>
        </td>
        </tr>
      </table>
    </div>
    
    <br><hr><h3>Picture</h3>
  <div><br><center><img src="saved-photo" id="photo" width="70%"></center></div>

   <hr>
   Project: ESP32 PrusaConnect Camera, Author: Miroslav Pivovarsky, e-mail: miroslav.pivovarsky@gmail.com
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

  function reset() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/reset", true);
    xhr.send();
  }

  function reboot() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/reboot", true);
    xhr.send();
  }

function automaticPhoto(cb) {

      var xhr = new XMLHttpRequest();
    xhr.open('GET', "/autophoto", cb.checked);
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
              if (obj.autoPhoto == '1')
{
    $( "#autoPhoto").prop('checked', true);
}
else
{
    $( "#autoPhoto").prop('checked', false);
}

              $("#photoquality").text(obj.photoquality);
              $("#framesize").text(obj.framesize);
              $("#brightness").text(obj.brightness);
              $("#contrast").text(obj.contrast);
              $("#saturation").text(obj.saturation);
              $("#hmirror").text(obj.hmirror);
              $("#vflip").text(obj.vflip);
              $("#lensc").text(obj.lensc);
              $("#exposure_ctrl").text(obj.exposure_ctrl);
              $("#led").text(obj.led);
              $("#rssi").text(obj.rssi);
              $("#camera_flash").text(obj.camera_flash);
              $("#camera_flash_duration").text(obj.camera_flash_duration);
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
