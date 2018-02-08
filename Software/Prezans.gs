function onOpen() {
  addPrezansMenu();
}

function addPrezansMenu(){
  var dp = PropertiesService.getDocumentProperties();
  var ss = SpreadsheetApp.getActiveSpreadsheet();  
  var sui = SpreadsheetApp.getUi();
  var menu = sui.createMenu("Prezans");
  
  menu.addItem('Update', 'update');
//  menu.addSeparator();
//  menu.addItem('Import', 'import');
//  menu.addSeparator();
//  menu.addItem('Attendence email', 'attendenceEmail');
//  menu.addSeparator();
//  menu.addItem('Send email', 'sendEmail');
  menu.addToUi();  
}

function update(){
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  
  var registersSheet = ss.getSheetByName('Registers');
  var attendenceSheet = ss.getSheetByName('Attendence');
  
  var registers = registersSheet.getRange(2, 1, registersSheet.getLastRow(), registersSheet.getLastColumn()).getValues();
  var uids = attendenceSheet.getRange(4, 1, attendenceSheet.getLastRow(), 1).getValues();
  
  var attendencesRange = attendenceSheet.getRange(3, 1, attendenceSheet.getLastRow(), attendenceSheet.getLastColumn() - 1);
  var attendences = attendencesRange.getValues();
  
  //Browser.msgBox('Error', registers[0][1],
  //      Browser.Buttons.OK);
  
  var headerAttend = {
    'UID': 0,
    'DateStart': 4,
  };
  
  var headerRegister = {
    'Date': 0,
    'Time': 1,
    'UID': 2
  };
  
  var indexedPresence = {
    //student: {
      //date: true
    //}
  };
  
  
  for(var line in registers){
    var data = registers[line];
    
    var uid = data[headerRegister['UID']];
    
    var date = data[headerRegister['Date']];
    
    if(typeof date != 'object')
      continue;
    
    date = date.toISOString().slice(0,10).replace(/-/g,"");
    
    // Save date in presence for student
    indexedPresence[uid] = indexedPresence[uid] || {};
    indexedPresence[uid][date] = true;
  }
  
  
  for(var line = 1; line < attendences.length; line++){
    var data = attendences[line];
    var uid = data[headerAttend['UID']];
    
    // Loop through date headers
    for(var col = headerAttend['DateStart']; col < data.length; col++){
      var thisDate = attendences[0][col].toISOString().slice(0,10).replace(/-/g,"");
      //Logger.log('UID: '+uid+' date: '+thisDate);
      
      var isPresent = indexedPresence[uid] && indexedPresence[uid][thisDate];
      Logger.log(uid+' is present:'+isPresent);
      if(isPresent)
        data[col] = 'P';
    }
  }
  
  attendencesRange.setValues(attendences);
}

function run() {
}