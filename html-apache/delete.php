<?php
	include('conn.php');
	$acrid=$_GET['ACRID']; // id of ACR to be deleted
	$acpid=$_GET['ACPID']; // id of ACP of the ACR to be deleted
	echo $acrid;
//	/*	
		$sql =<<<EOF
   DELETE FROM ACRt WHERE ACRID = '$acrid';
EOF;

   $ret = $db->query($sql);
   
	if(!$ret)
      echo "<br /><hr /> &nbsp " . $db->lastErrorMsg();
//	*/
// delete the resource if the ACPID has no ACR left

		$sql =<<<EOF
   SELECT ACPID FROM ACRt WHERE ACPID = '$acpid' ORDER BY ACPID DESC LIMIT 1;
EOF;
   $ret = $db->query($sql);
	if(!$ret)
      echo "<br /><hr /> &nbsp " . $db->lastErrorMsg();
	$row = $ret->fetchArray(SQLITE3_ASSOC);
	if ($row['ACPID'])
	{
		echo "Do nothing";
	}
	else
	{
		echo "delete resource row";
		$sql =<<<EOF
   DELETE FROM resource WHERE ACPID = '$acpid';
EOF;
   $ret = $db->query($sql);
	if(!$ret)
      echo "<br /><hr /> &nbsp " . $db->lastErrorMsg();
	}
	
	header('location:index.php');

?>