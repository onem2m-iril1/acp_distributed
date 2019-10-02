<?php
	if (empty($_POST))
	{
		echo "Please fill data appropriately.!\n";
		die();
	}
	include('conn.php');	
	$acrid = $_POST['acrid'];
	$fr	=	$_POST['fr'];
	$op	=	$_POST['op'];
	$ty	=	$_POST['ty'];
	$ty1	=	$_POST['ty1'];
	$acrid = $_POST['acrid'];
	echo "ty1 = " . $ty1 . "ACRID = " . $acrid . "<br />From = " . $fr . "<br />op = " . $op . "<br />ty = " . $ty;
	
	$sql =<<<EOF
   UPDATE ACRt SET fr='$fr', op='$op', ty='$ty' WHERE ACRID='$acrid';
EOF;
	$ret = $db->query($sql);
	$row = $ret->fetchArray(SQLITE3_ASSOC);
	
	if(!$ret)
      echo "<br /><hr />" . $db->lastErrorMsg();
	else
		echo "<br /><hr />Edited.!";

	
	header('location:index.php');

?>