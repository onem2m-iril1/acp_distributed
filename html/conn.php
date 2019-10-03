
<?php
	 class MyDB extends SQLite3 {
      function __construct() {
//         $this->open('test.db');
         $this->open('/home/pi/temp01/PDPserver/test.db');
      }
   }  
   $db = new MyDB();
   if(!$db) {
      echo $db->lastErrorMsg();
   }
/*
   */ 
//   else { echo "Opened database successfully\n";   }
?>
