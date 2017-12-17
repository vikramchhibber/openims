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
 * standards. Thus all Open IMS Core users have to take notice of this 
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
 * I-CSCF Provisioning - Different usefull functions
 * 
 * 
 *  \author Dragos Vingarzan   dragos dot  vingarzan -at- fokus dot fraunhofer dot de
 * 
 */
 
function page_open($name)
{
	global $config;
	?>
<html>
<head>
<title>IMS Provisioning @ <? print $config->IMS_NAME; ?></title>
<? print $config->css_style; ?>
</head>
<body  leftmargin=0 topmargin=0>
  <table align=center valign=middle height=100%>
    <tr>
      <td align=center><h1><br><?print $name; ?></h1></td>
	</tr>
	<tr height=99%>
	  <td align=center>
 <?	  
}

function page_close()
{
	?>				
	 </td>
    </tr>
  </table>

</body>
</html>
 <?
}


function repeat_search($post,$f)
{
	foreach($post as $k=>$v){
		if (substr($k,0,4)=="src_"){
			$f->add_element(array("type"=>"hidden",
	    	              "name"=>$k,
			      "value"=>$v));
	//		$f->show_element($k);
	//		print($k."=".$v." | ");
		}
	}
}


function page_navigation($page,$total)
{
	global $result;

 	//print $page;
 	if ($page>0) $prev = $page-1;
 	else $prev = 0;
 	if ($page < $result->pages-1) $next = $page+1;
 	else $next = $result->pages-1;
 	
	?>
	 	<tr>
 		<td align=left width=30%>
 		 <?
 		 	print("<a href=# " .
 		 			"onClick=\"document.forms[0].src_page.value=0".
					"; document.forms[0].submit(); \">|&lt;</a>&nbsp;&nbsp;&nbsp;");
 		 	print("<a href=# " .
 		 			"onClick=\"document.forms[0].src_page.value=".($prev).
					"; document.forms[0].submit(); \">&lt;&lt;</a>");
		 ?>
 		</td>
 		<td align=center width=40%><? print"Page ".($_POST["src_page"]+1)." of ".$total." "; ?></td>
 		<td align=right width=30%>
 		 <?
 		 	print("<a href=# " .
 		 			"onClick=\"document.forms[0].src_page.value=".
 		 				$next.
					"; document.forms[0].submit(); \">&gt;&gt;</a>&nbsp;&nbsp;&nbsp;");
 		 	print("<a href=# " .
 		 			"onClick=\"document.forms[0].src_page.value=".($total-1).
					"; document.forms[0].submit(); \">&gt;|</a>");
		 ?>
 		</td> 	 	
 	</tr>
	<?
}


?>
