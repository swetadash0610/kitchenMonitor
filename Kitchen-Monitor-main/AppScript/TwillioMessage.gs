// const YOURACCOUNTSID="AC3763ecf561259695b00b1ea40817dd26";
// const YOURTWILIONUMBER="+13803339038";
// const YOURAUTHTOKEN="4df277b4015884afffe01cde8ed41b29";

function sendSms(to, body) {
  var messages_url = "https://api.twilio.com/2010-04-01/Accounts/AC3763ecf561259695b00b1ea40817dd26/Messages.json";

  var payload = {
    "To": to,
    "Body" : body,
    "From" : "+13803339038"
  };

  var options = {
    "method" : "post",
    "payload" : payload
  };

  options.headers = { 
    "Authorization" : "Basic " + Utilities.base64Encode("AC3763ecf561259695b00b1ea40817dd26:4df277b4015884afffe01cde8ed41b29")
  };

  UrlFetchApp.fetch(messages_url, options);
}

function sendAll() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var startRow = 2; 
  var numRows = sheet.getLastRow() - 1; 
  var dataRange = sheet.getRange(startRow, 1, numRows, 6) 
  var data = dataRange.getValues();

  for (i in data) {
    var row = data[i];
    if(row[5].match(/\d/g).join("")<30){
      // try {
      sendSms("+917735806945", `${row[1]} content is nearly ${row[5]}.You might consider buying it before its over!`);
    // } catch(err) {
    //   Logger.log(err);
    // }
    }
  }
}

function myFunction() {
  sendAll();
}
