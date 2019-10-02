<?php
	if (empty($_POST))
	{
		echo "Please fill data appropriately.!\n";
		die();
	}
	include('conn.php');
	$resid = $_POST['resid'];
	$fr	=	$_POST['fr'];
	$op	=	$_POST['op'];
	$ty	=	$_POST['ty'];
	echo "Resource ID = " . $resid . "<br />From = " . $fr . "<br />op = " . $op . "<br />ty = " . $ty;
	
	$sql =<<<EOF
   SELECT ACPID FROM ACRt ORDER BY ACPID DESC LIMIT 1;
EOF;
	$ret = $db->query($sql);
	$row = $ret->fetchArray(SQLITE3_ASSOC);
//echo "ACPID= " . $row[ACPID];
//	die();
	$acpid = $row[ACPID] +1;
//	echo "ACPID= " . $acpid;
//die();
// insert into ACRt
	$sql =<<<EOF
      INSERT INTO ACRt (ACPID, fr, op, ty) VALUES ('$acpid', '$fr', '$op', '$ty');
EOF;
   $ret = $db->query($sql);   
// insert into resource
	$sql =<<<EOF
      INSERT INTO resource (ResID, ACPID) VALUES ('$resid', '$acpid');
EOF;
   $ret = $db->query($sql);

//	mysqli_query($conn,"insert into resource (resid, ACPID) values ('$resid', '$acpid')");
//	mysqli_query($conn,"insert into ACRt (ACPID, fr, op, ty) values ('$acpid''$fr', '$op', '$ty')");
	if(!$ret)
      echo "<br /><hr />" . $db->lastErrorMsg();
	else
		echo "<br /><hr />added.!";
	header('location:index.php');

?>