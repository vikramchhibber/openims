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
 * I-CSCF Provisioning -   DB Operations
 * 
 * 
 *  \author Dragos Vingarzan   dragos dot  vingarzan -at- fokus dot fraunhofer dot de
 * 
 */

class Data_Layer_icscf {
	var $link;
	
	function Data_Layer()
	{
		
	}
	
	function init()
	{
		global $config;
		$link = mysql_connect($config->dbhost_icscf,$config->dbuser_icscf,$config->dbpass_icscf)
    		or die("Could not connect : " . mysql_error());
		mysql_select_db($config->dbname_icscf,$link) or die("Could not select database");
	}

	function close()
	{
		mysql_close();
	}	

	function get_trusted_list($post)
	{
		global $config;
		
		$r->total_rows=0;
		$r->data=array();
		
		if ($post["src_order"]==null) $post["src_order"]="identity";
		if ($post["src_page"]==null) $post["src_page"]=0;
		
		$fields = "id,trusted_domain ";
		$cond = "from nds_trusted_domains ".
			    " where 1 ";
		if (strlen($post["src_id"])>0)
			$cond.=" and id='".mysql_real_escape_string($post["src_id"])."'";

		if (strlen($post["src_trusted_domain"])>0)
			$cond.=" and trusted_domain like '%".mysql_real_escape_string($post["src_trusted_domain"])."%'";

		$query = "select count(*) ".$cond;			
		$result = mysql_query($query) or die("Query failed : " . mysql_error());
		$line = mysql_fetch_array($result, MYSQL_NUM);
		$r->total_rows = $line[0];
		$r->pages = ceil($r->total_rows/$config->results_per_page);
		mysql_free_result($result);

		$query = "select ".$fields." ".$cond." ".
				"order by ".$post["src_order"]." ".
				" limit ".($post["src_page"]*$config->results_per_page).",".$config->results_per_page.";";			
		$result = mysql_query($query) or die("Query failed : " . mysql_error());

		while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    		$r->data[]=$line;
		}
		mysql_free_result($result);
		return $r;
	}
		
	function get_scscf_list($post)
	{
		global $config;
		
		$r->total_rows=0;
		$r->data=array();
		
		if ($post["src_order"]==null) $post["src_order"]="name";
		if ($post["src_page"]==null) $post["src_page"]=0;
		
		$fields = "id,name,s_cscf_uri as uri ";
		$cond = "from s_cscf ".
			    " where 1 ";
		if (strlen($post["src_id"])>0)
			$cond.=" and id='".mysql_real_escape_string($post["src_id"])."'";

		if (strlen($post["src_name"])>0)
			$cond.=" and name like '%".mysql_real_escape_string($post["src_name"])."%'";

		if (strlen($post["src_uri"])>0)
			$cond.=" and s_cscf_uri like '%".mysql_real_escape_string($post["src_uri"])."%'";

		$query = "select count(*) ".$cond;			
		$result = mysql_query($query) or die("Query failed : " . mysql_error());
		$line = mysql_fetch_array($result, MYSQL_NUM);
		$r->total_rows = $line[0];
		$r->pages = ceil($r->total_rows/$config->results_per_page);
		mysql_free_result($result);

		$query = "select ".$fields." ".$cond." ".
				"order by ".$post["src_order"]." ".
				" limit ".($post["src_page"]*$config->results_per_page).",".$config->results_per_page.";";			
		$result = mysql_query($query) or die("Query failed : " . mysql_error());

		while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    		$r->data[]=$line;
		}
		mysql_free_result($result);
		return $r;
	}	
	function get_trusted($post)
	{
		global $config;		
		$fields = " id,trusted_domain ";
		$cond = "from nds_trusted_domains ".
			    " where id='".$post["key"]."' ";

		$query = "select ".$fields." ".$cond." ".
				" limit 1;";
		//print $query;
							
		$result = mysql_query($query) or die("Query failed : " . mysql_error());

		if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    		$r=$line;
		}
		mysql_free_result($result);
		return $r;
	}	

	function get_scscf($post)
	{
		global $config;		
		$fields = " id,name,s_cscf_uri as uri ";
		$cond = "from s_cscf ".
			    " where id='".$post["key"]."' ";

		$query = "select ".$fields." ".$cond." ".
				" limit 1;";
		//print $query;
							
		$result = mysql_query($query) or die("Query failed : " . mysql_error());

		if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    		$r=$line;
		}
		mysql_free_result($result);
		return $r;
	}		
	function set_trusted($post)
	{
		$query = "update nds_trusted_domains set " .
				" trusted_domain='".mysql_real_escape_string($post["trusted_domain"])."' ".				
				" where id = '".$post["id"]."'; ";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on update!</b></font>";
			
		$r = "Updated ".mysql_affected_rows()." rows.";
		return "<b>".$r."</b>";
	}	
	function set_scscf($post)
	{
		$query = "update s_cscf set " .
				" name='".mysql_real_escape_string($post["name"])."', ".				
				" s_cscf_uri='".mysql_real_escape_string($post["uri"])."' ".				
				" where id = '".$post["id"]."'; ";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on update!</b></font>";
			
		$r = "Updated ".mysql_affected_rows()." rows.";
		return "<b>".$r."</b>";
	}			
	function new_trusted($post)
	{
		global $_POST;
		
		$query = "insert into nds_trusted_domains (trusted_domain) values ('".
			mysql_real_escape_string($post["new_trusted_domain"])."');";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on insert!</b></font>";
			
		$r = "Inserted ".mysql_affected_rows()." row. Id: ".mysql_insert_id();
		$_POST["id"]=mysql_insert_id();
		$_POST["key"]=mysql_insert_id();
		return "<b>".$r."</b>";
	}		
	
	function new_scscf($post)
	{
		global $_POST;
		
		$query = "insert into s_cscf (name) values ('".
			mysql_real_escape_string($post["new_name"])."');";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on insert!</b></font>";
			
		$r = "Inserted ".mysql_affected_rows()." row. Id: ".mysql_insert_id();
		$_POST["id"]=mysql_insert_id();
		$_POST["key"]=mysql_insert_id();
		return "<b>".$r."</b>";
	}		

	function del_trusted($post)
	{
		global $_POST;
		$id = $_POST["id"];
		
		mysql_query("delete from nds_trusted_domains where id=".$id);			
		$r .= "Droped ".mysql_affected_rows()." Trusted Domains.";
				
		return "<b>".$r."</b>";
	}		
	
	function del_scscf($post)
	{
		global $_POST;
		$id = $_POST["id"];
		
		mysql_query("delete from s_cscf_capabilities where id_s_cscf=".$id);			
		$r .= "Droped ".mysql_affected_rows()." Useless Capabilities.";
		mysql_query("delete from s_cscf where id=".$id);			
		$r .= "Droped ".mysql_affected_rows()." S-CSCF.";
				
		return "<b>".$r."</b>";
	}	
	function get_scscf_capabilities($id)
	{
		global $config;
		$r = array();		
		$fields = " id,capability ";
		$cond = "from s_cscf_capabilities ".
				" where id_s_cscf='".$id."' ";
		$query = "select ".$fields." ".$cond." ".
				" order by capability;";
						
		$result = mysql_query($query) or die("Query failed : " . mysql_error());

		while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    		$r[]=$line;
		}
		mysql_free_result($result);
		return $r;
	}	
	function set_scscf_capability($post)
	{
		$id = $post["id"];
		$capability = $post["new_capab"];
		$result = mysql_query("select count(*) from s_cscf_capabilities" .
				" where id_s_cscf='".$id."'".
				" and capability='".$capability."'")
			or die("Query failed : " . mysql_error());
		if ($line = mysql_fetch_array($result, MYSQL_NUM)) {
    		if ($line[0]>0)
    		return "<font color=#ff0000><b>Capability already attached!</b></font>";
		}else {
			return "<font color=#ff0000><b>Error checking for duplicates!</b></font>";
		}
			
		$query = " insert into s_cscf_capabilities (id_s_cscf,capability)" .
				" values ('".$id."','".$capability."');";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on insert!</b></font>";
		$r = "Inserted ".mysql_affected_rows()." row.";
		return "<b>".$r."</b>";
	}
	function del_scscf_capability($post)
	{
		$id = $post["id"];
		$capability = $post["capab"];
		
		$query = " delete from s_cscf_capabilities where id_s_cscf='".$id.
				"' and capability='".$capability."' ";
		if (!mysql_query($query))
			return "<font color=#ff0000><b>Error on delete!</b></font>";
		$r = "Deleted ".mysql_affected_rows()." rows.";
		return "<b>".$r."</b>";
	}

}



?>
