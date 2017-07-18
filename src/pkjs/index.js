/*jshint esversion: 6 */

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

var passedOrFailed = function(result, obj) {
	if(obj.LastOutcome == "Passed") {
  	result.passed++;
  } else {
  	result.failed++;
  }
};

function getTestsData() {
  // Construct URL
  var url = 'INSERT URL HERE';

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var data = JSON.parse(responseText);
      var result = {
      	passed: 0,
        failed: 0
      };
      
      Object.entries(data).forEach(
      		([key, value]) => passedOrFailed(result, data[key])
        );
      
      var passed = result.passed;
      var failed = result.failed;
      var date = new Date(data[Object.keys(data)[0]].LastExecuted);
      var dateExecuted = date.getDate() + "." + (date.getMonth() + 1) + "." +date.getFullYear();
      var timeExecuted = date.getHours() + ":" + date.getMinutes();
      
      // Assemble dictionary using our keys
      var dictionary = {
        'PASSED': passed,
        'FAILED': failed,
        'LASTEXECUTEDDATE': dateExecuted,
        'LASTEXECUTEDTIME': timeExecuted
      };
      
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Info sent to Pebble successfully!");
        }, function(e) {
          console.log("Error sending info to Pebble!");
        }
      );     
      
    }      
  );
}


// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    getTestsData();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getTestsData();
  }                     
);