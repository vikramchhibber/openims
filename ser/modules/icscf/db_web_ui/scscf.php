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
 * I-CSCF Provisioning -   S-CSCF Page
 * 
 * 
 *  \author Dragos Vingarzan   dragos dot  vingarzan -at- fokus dot fraunhofer dot de
 * 
 */

include "config.inc"; 
include "pre_icscf.inc"; 



function search_form()
{
	global $options_registered;
	
	$f = new form;
	
	$f->start("form");
	?>
	<table border=0 cellspacing=0 align=center width=300>
	 <tr><td align=right><b>Enter Search Parameters</b></td></tr>
	 <tr><td>
	 <table border="0" cellspacing="1" align=center width="100%" style="border:1px solid #4444ff;">
	  <tr bgcolor="#eeeeee"><td><label>ID</label></td>
	  	<?
			$f->add_element(array("type"=>"text",
	                          "name"=>"src_id",
							  "value"=>$_POST["src_id"],
						 	  "extrahtml"=>"alt='ID from database' size=8"));
		?>
	  	<td bgcolor="#dddddd"><? $f->show_element("src_id");?></td>
	  </tr>
	
	  <tr bgcolor="#eeeeee"><td><label>Name</label></td>
	  	<?
			$f->add_element(array("type"=>"text",
	                          "name"=>"src_name",
							  "value"=>$_POST["src_name"],
						 	  "extrahtml"=>" size=32"));
		?>
	  	<td bgcolor="#dddddd"><? $f->show_element("src_name");?></td>
	  </tr>
	  
	  <tr bgcolor="#eeeeee"><td><label>URI</label></td>
	  	<?
			$f->add_element(array("type"=>"text",
	                          "name"=>"src_uri",
							  "value"=>$_POST["src_uri"],
						 	  "extrahtml"=>" size=32"));
		?>
	  	<td bgcolor="#dddddd"><? $f->show_element("src_uri");?></td>
	  </tr>		
	  <tr>
	   <td colspan=2 align=center>
		<table border=0 cellspacing=0 align=center width=100%>
	   	 <tr bgcolor="#ddffdd">
			<td w>&nbsp;</td>
	<?
		$f->add_element(array("type"=>"submit",
	    	              "name"=>"search",
						  "value"=>"Search ->",
						  "extrahtml"=>"alt='Search with above parameters'"));
	?>
			<td align="right"><?$f->show_element("search");?></td>
		 </tr>
	    </table>
	   </td>
	  </tr>
	 </table>
	</td></tr>
	</table>
	<?
	
	$f->add_element(array("type"=>"hidden",
	    	          "name"=>"src_do",
					  "value"=>"search"));
	$f->show_element("src_do");
	$f->add_element(array("type"=>"hidden",
	    	          "name"=>"src_page",
					  "value"=>"0"));
	$f->show_element("src_page");
	$f->add_element(array("type"=>"hidden",
	    	          "name"=>"src_order",
					  "value"=>"name"));
	$f->show_element("src_order");
	
	$f->finish("",null);
	
}


function search_result($result)
{
	global $lastop,$lastopcap;
	$f = new form();
	$f->start("form");
	$f->add_element(array("type"=>"submit",
	    	              "name"=>"newsearch",
						  "value"=>"<- New Search",
						  "extrahtml"=>"alt='Go back and search again' onClick='src_do.value=0'"));
	$f->add_element(array("type"=>"submit",
	    	              "name"=>"refresh",
						  "value"=>"Refresh",
						  "extrahtml"=>"alt=\"Refresh this page\""));
	$f->add_element(array("type"=>"hidden",
	    	              "name"=>"key",
						  "value"=>"0"));
	$f->show_element("id");						  
	repeat_search($_POST,$f);
 ?>	
	<table border=0 cellspacing=0 align=center width=300>
	 <?
	 	if (isset($lastop))
	 		print("<tr><td colspan=3 align=center>".$lastop."</td></tr>");
	 ?>
	 <tr>
	 	<td align="left" width=33%><? $f->show_element("newsearch"); ?></td>
	 	<td align="center" width=33%><? $f->show_element("refresh"); ?></td>
	 	<td align=right ><b>Search Results</b></td></tr>
	 <tr><td colspan=3>
	 <table border="0" cellspacing="1" align=center width="100%" style="border:1px solid #4444ff;">
	  <tr bgcolor=#ddddff>
	  <script language=javascript>
	  	function order(x){
	  		if (document.forms[0].src_order.value!=x)
	  			document.forms[0].src_order.value=x;
	  		else document.forms[0].src_order.value=x+' desc';
	  		document.forms[0].submit();
	  	}
	  </script>
	  	<td><b><a href=# onClick="order('id');">ID</a></b></td>
	  	<td><b><a href=# onClick="order('name');">Name</a></b></td>
	  	<td><b><a href=# onClick="order('s_cscf_uri');">URI</a></b></td>
	  </tr>
	  <script language=javascript>
	  	function edit(i){
	  		document.forms[0].key.value=i;	  		
 			document.forms[0].src_do.value='edit';
	  		document.forms[0].submit();
	  	}
	  </script>
 <?
 	foreach($result->data as $k=>$v){
 		print("<tr bgcolor=#eeeeff>");
 		print("<td align=right>".$v["id"]."</td>");
 		print("<td align=left><a href=# onClick=\" edit(".$v["id"].");\">".
	  			$v["name"]."</a></td>");
 		print("<td align=left>".$v["uri"]."</a></td>");
 		print("</tr>");
 	}
 	

?>	  <script language=javascript>
	  	function newscscf(){
	  		document.forms[0].src_do.value='new';
	  		document.forms[0].submit();
	  	}
	  </script>
<? 	
	$f->add_element(array("type"=>"text",
                          "name"=>"new_name",
						  "value"=>$_POST["new_name"],
					 	  "extrahtml"=>" size=32"));
	 	
 	print("<tr bgcolor=#eeeeff><td></td>");
 	print("<td bgcolor=#00ff00 colspan=2>");$f->show_element("new_name");print("&nbsp;&nbsp;");
 	print("<input type=button name=\"new\" value=\"new\" onClick=\" newscscf();\"></td>"); 	
 	
 	print("</tr>");
 	
 	//print_r($result);
	?>
 	</table>
 	</td><tr>
 	<?
	page_navigation($_POST["src_page"],$result->pages);
	$f->finish("",null);		
}

function edit_form($result)
{
	global $data;
	global $lastop,$lastopcap;

	$f = new form();
	$f->start("form");
	$f->add_element(array("type"=>"submit",
	    	              "name"=>"back",
						  "value"=>"<- Back to List",
						  "extrahtml"=>"alt=\"Go back to the list\" onClick=\"src_do.value='search';\""));
	$f->add_element(array("type"=>"submit",
	    	              "name"=>"refresh",
						  "value"=>"Refresh",
						  "extrahtml"=>"alt=\"Refresh this page\" onClick=\"src_do.value='edit';\""));
	$f->add_element(array("type"=>"hidden",
	    	              "name"=>"key",
						  "value"=>$_POST["key"]));
	$f->show_element("id");						  
	repeat_search($_POST,$f);

//	print "edit";
//	print_r($_POST);
//	print_r($result);
?>	
	<table border=0 cellspacing=0 align=center width=500>

	 <tr>
	 	<td align="left" width=50%><? $f->show_element("back"); ?></td>
	 	<td align="center" width=50% colspan=2><? $f->show_element("refresh"); ?></td>
	 
	 <tr><td colspan=1>
	 <table border="0" cellspacing="1" align=center style="border:1px solid #4444ff;">
	 	<tr><td colspan=2 align=right><b>Edit S-CSCF</b></td></tr>
	   	<?
/* EDIT */	   	
	   	if (isset($lastop))
	   		print ("<tr><td colspan=2 align=center>".$lastop."</td></tr>");
		$f->add_element(array("type"=>"text",
	                          "name"=>"id",
							  "value"=>$result["id"],
						 	  "extrahtml"=>"readonly alt='ID from database' size=8"));
		$f->add_element(array("type"=>"text",
	                          "name"=>"name",
							  "value"=>$result["name"],
						 	  "extrahtml"=>"alt='Name' size=16"));
		$f->add_element(array("type"=>"text",
	                          "name"=>"uri",
							  "value"=>$result["uri"],
						 	  "extrahtml"=>"alt='SIP URI' size=32"));
		
		$f->add_element(array("type"=>"submit",
		    	              "name"=>"update",
							  "value"=>"Update",
							  "extrahtml"=>"alt=\"Update this values\" onClick=\"src_do.value='update';\""));
		?>				 	  
		<tr bgcolor=#ddddff>
			<td><label>ID</label></td><td><? $f->show_element("id"); ?></td>
		</tr>
		<tr bgcolor=#ddddff>
			<td><label>Name</label></td><td><? $f->show_element("name"); ?></td>
		</tr>
		<tr bgcolor=#ddddff>
			<td><label>URI</label></td><td><? $f->show_element("uri"); ?></td>
		</tr>
		<tr bgcolor=#ddddff>
			<td bgcolor=#00ff00 align=center><? $f->show_element("update"); ?></td>
			<td bgcolor=#ff0000 align=center><input type=button name=del value="Delete" 
				onClick="document.forms[0].src_do.value='del';document.forms[0].submit();"></td>
		</tr>
	</table>
	</td>
<? 
/* Capabilities */
?>	 
	 <td colspan=1>
	  <table border="0" cellspacing="1" align=center style="border:1px solid #4444ff;">
	  	<tr><td colspan=3 align=right><b>Capabilities</b></td></tr>
<?
	   	if (isset($lastopcap))
	   		print ("<tr><td colspan=2 align=center>".$lastopcap."</td></tr>");

?>	   <tr bgcolor=#ddddff>
	   	<td><b>Capability</b></td>
	   	<td></td>
	   </tr>
	  <script language=javascript>
	  	function delCap(i){
	  		document.forms[0].capab.value=i;	  		
 			document.forms[0].src_do.value='delcap';
	  		document.forms[0].submit();
	  	}
	  	function addCap(){
 			document.forms[0].src_do.value='addcap';
	  		document.forms[0].submit();
	  	}
	  </script>
	   <?
		$f->add_element(array("type"=>"hidden",
		    	              "name"=>"capab",
							  "value"=>$_POST["capab"]));
		$f->add_element(array("type"=>"text",
		    	              "name"=>"new_capab",
							  "value"=>$_POST["new_capab"],
							  "extrahtml"=>"size=4"));
		
		$capa = $data->get_scscf_capabilities($result["id"]);
		foreach($capa as $k=>$v){
			print("<tr bgcolor=#eeeeff>");
			print("<td align=right>".$v["capability"]."</td>");
			print("<td align=center bgcolor=#ff0000>" .
				"<input type=button name=del value=\"del\" onClick=\"delCap(".$v["capability"].");\">" .
				"</td>");
			print("</tr>");	
		}
		print("<tr bgcolor=#00ff00>");
		print("<td colspan=1>");
		$f->show_element("new_capab");
		print("</td>");
		print("<td align=center bgcolor=#00ff00>" .
				"<input type=button name=add value=\"add\" onClick=\"addCap();\">" .
				"</td>");
		print("</tr>")		
		?>
	  </table>
	 </td>		

	 </tr>
	</table>
<?	 

	$f->finish("",null);		

}




// PAGE GENERATION

page_open("Serving-CSCF Capability Configuration");


switch($do){
	case "search":
		$result = $data->get_scscf_list($_POST);
		search_result($result); 
		break;	
	case "edit":
		$result = $data->get_scscf($_POST);
		edit_form($result); 
		break;	
	case "update":
		$lastop = $data->set_scscf($_POST);
		$result = $data->get_scscf($_POST);
		edit_form($result); 
		break;			
	case "new":
		$lastop = $data->new_scscf($_POST);
		$result = $data->get_scscf($_POST);
		edit_form($result); 
		break;			
	case "del":
		$lastop = $data->del_scscf($_POST);
		$result = $data->get_scscf_list($_POST);
		search_result($result); 
		break;			
	case "addcap":
		$lastopcap = $data->set_scscf_capability($_POST);
		$result = $data->get_scscf($_POST);
		edit_form($result); 
		break;			
	case "delcap":
		$lastopcap = $data->del_scscf_capability($_POST,1);
		$result = $data->get_scscf($_POST);
		edit_form($result); 
		break;			
		
	default:
		search_form();
}

page_close();

include "post.inc";
