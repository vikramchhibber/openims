<?php
/*
 * $Id$
 *  
 * Copyright (C) 2004-2008 FhG Fokus
 *
 * This file is part of Open IMS Core - an open source IMS CSCFs & HSS
 * implementation
 *
 * Open IMS Core is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * For a license to use the Open IMS Core software under conditions
 * other than those described here, or to purchase support for this
 * software, please contact Fraunhofer FOKUS by e-mail at the following
 * addresses:
 *     info@open-ims.org
 *
 * Open IMS Core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * It has to be noted that this Open Source IMS Core System is not 
 * intended to become or act as a product in a commercial context! Its 
 * sole purpose is to provide an IMS core reference implementation for 
 * IMS technology testing and IMS application prototyping for research 
 * purposes, typically performed in IMS test-beds.
 * 
 * Users of the Open Source IMS Core System have to be aware that IMS
 * technology may be subject of patents and licence terms, as being 
 * specified within the various IMS-related IETF, ITU-T, ETSI, and 3GPP
 * standards.l Open IMS Core users have to take notice of this 
 * fact and have to agree to check out carefully before installing, 
 * using and extending the Open Source IMS Core System, if related 
 * patents and licences may become applicable to the intended usage 
 * context.  
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */
 
/**
 * \file
 * 
 * I-CSCF Provisioning - Top Menu
 * 
 * 
 *  \author Dragos Vingarzan   dragos dot  vingarzan -at- fokus dot fraunhofer dot de
 * 
 */

include "config.inc"; 

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
<link rel="stylesheet" type="text/css" href="style/fokus_ngni.css">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Open IMS Core Provisioning @ <? print $config->HSS_NAME; ?></title>
</head>
<body>
<table id="navigation" height="100%">
	<tr id="bound_bottom">
		<td id="bound_left">&nbsp;</td>
		<td><a href="http://www.fokus.fraunhofer.de/en/ngni/index.html" target="_blank"><img
			src="images/hauptlogo_01_en.gif" /></a> <br>

		</td>
		<td><img src="images/blind.gif" height="73" width="575" /></td>
		<td><a
			href="http://www.fokus.fraunhofer.de/en/ngni/services/testbeds/index.html" target="_blank"><img
			src="images/hauptlogo_02_en.gif" /></a>
		</td>
		<td id="bound_right">&nbsp;</td>
	</tr>
	<tr height="30" bgcolor="#FFFFFF" >
		<td id="bound_left">&nbsp;</td>
		<td colspan="3" id="header">

			<a href="http://www.openimscore.org">Open IMS Core<a>
		</td>
		<td id="bound_right">&nbsp;</td>
	</tr>
	<tr id="bound_bottom">
		<td id="bound_left">&nbsp;</td>
		<td colspan="3">&nbsp;</td>
		<td id="bound_right">&nbsp;</td>
	</tr>

	<tr id="bound_bottom">
		<td id="bound_left">&nbsp;</td>
		<td id="level01" colspan="2">
			<a href="index_main.php" target="main_frame">Provisioning</a>
			
		</td>
		<td id="language"><a href="javascript:alert('Try again later!');">help</a></td>
		<td id="bound_right">&nbsp;</td>
	</tr>
	<tr id="bound_bottom_black" bgcolor="#EB9015" >
		<td id="bound_left">&nbsp;</td>
		<td id="level02" colspan="3">&nbsp;</td>

		<td id="bound_right">&nbsp;</td>
	</tr>
</table>
</body>
</html>


