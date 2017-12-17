<?php
/* OOHForms: Date 29-09-2000
 *
 * Vinay Kumar - vinay@sanisoft.com
 * Girish Nair - girish@sanisoft.com
 * Copyright (c) 2000 by SANIsoft
 *
 * Date fromat - dd-mm-YYYY
 *
 * validate() must be called!!!
 *
 * Changes on -- 26-oct-2002
 * Changed document.forms[thisForm][thisday].focus()
 * to f.'.$this->name.'_day.focus();
 * Error detected by - Derek Scruggs <derek@creative-mail.com>
 *
 *
 * Changes on 24-Feb-2003
 * Added format attribute - d/m/y
 * where alphabet has the same meaning as that of PHP date function
 * Month format allowed:
     F - long month names,
     M - Short (3-char) month names
     m - Numeric months preceeded by zero
     n - Numeric months
 * The position of select boxes can also be swapped.
 */

class of_calendar extends of_element {

  var $options;
  var $size = "";
  var $valid_e = "";
  var $comp_e = "";
  var $day_arr;
  var $year_arr;
  var $year_from = 1950;
  var $year_to = 2020;
  var $day = "";
  var $month = "";
  var $year = "";
  var $dayv = "";
  var $monthv = "";
  var $yearv = "";

  var $multiple=false;
  var $month_rarr = array("F" => array("January","February","March","April","May",
                                      "June","July","August","September","October","November","December"),
                         "M" => array("Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"),

                         "m" => array("01","02","03","04","05","06","07","08","09","10","11","12"),
                         "n" => array("1","2","3","4","5","6","7","8","9","10","11","12")
                         );
    var $dayPos = 1;
    var $monthPos = 0;
    var $yearPos = 2;

    var $js_function; /*** contains js validator function for the element ***/

// Constructor
  function of_calendar($a) {
    $this->day= date("d");
    $this->month=date("m");
    $this->year= date("Y");

    $this->setup_element($a);
    if(isset($a["year_from"])) $this->year_from=$a["year_from"];
    if(isset($a["year_to"])) $this->year_to=$a["year_to"];
    if(isset($a["value"])) {
        $val_arr=explode("-",$this->value);
        $this->dayv=$val_arr[0];
        $this->monthv=$val_arr[1];
        $this->yearv=$val_arr[2];
    }

    // display format
    if(!isset($a["format"])) {
        $a["format"] = "d/m/y";
    }
        //dd/mm/yyyy
        $formatArr = explode("/",$a["format"]);
        if(array_search("d",$formatArr)!== false) {
            $this->dayPos = array_search("d",$formatArr);
        }

        if(ereg("n|m|M|F",$a["format"])) {
            if(array_search("m",$formatArr)) {
                $this->monthPos = array_search("m",$formatArr);
            } else if (array_search("M",$formatArr)) {
                $this->monthPos = array_search("M",$formatArr);
            } else if(array_search("F",$formatArr)) {
                $this->monthPos = array_search("F",$formatArr);
            } else if(array_search("n",$formatArr)) {
                $this->monthPos = array_search("n",$formatArr);
            }
            $this->monthFormat = $formatArr[$this->monthPos];
        } else {
           // month not given
           //$this->monthPos = -1;
           $this->monthFormat = "m";
        }

        if(array_search("y",$formatArr)!== false) {
            $this->yearPos = array_search("y",$formatArr);
        }

//        echo "<br>Month Fromat:".$this->monthFormat;

    $this->js_function = '
      function checkCalendar(f,calName,year_from,year_to,comp_e,valid_e)  {
      var monthArr = new Array("January","February","March","April","May","June","July","August","September","October","November","December");

      if(f.elements[calName+"_month"].selectedIndex != null && f.elements[calName+"_day"].selectedIndex!=null && f.elements[calName+"_year"].selectedIndex!=null) {
        thisyear=calName+"_year";
        thismonth=calName+"_month";
        thisday=calName+"_day";

        var month=f.elements[thismonth].selectedIndex;
        var monthname=f.elements[thismonth].options[month].text;
        var monthvalue=f.elements[thismonth].options[month].value;

        var day=f.elements[thisday].selectedIndex;
        var dayname=f.elements[thisday].options[day].text;
        var dayvalue=f.elements[thisday].options[day].value;

        var year=f.elements[thisyear].selectedIndex;
        var yearname=f.elements[thisyear].options[year].text;
        var yearvalue=f.elements[thisyear].options[year].value;


      if(comp_e) {
        if(!(monthvalue > 0 && monthvalue < 13)) {
           monthvalue=-1;
        }
        if(!(dayvalue > 0 && dayvalue < 32)) {
           dayvalue=-1;
        }
        if(!(yearvalue >= year_from && yearvalue <= year_to)) {
           yearvalue=-1;
        }

        if (monthvalue==-1 || dayvalue==-1 || yearvalue==-1) {
            f.elements[thisday].focus();
            alert(comp_e);
            return false;
        }
      }


        yearmod=yearvalue%4;

        if (monthvalue==1 || monthvalue==3 || monthvalue==5 || monthvalue==7 || monthvalue==8 || monthvalue==10 || monthvalue==12)
                {
                if (dayvalue>31)
                        {
                            alert (valid_e+"\n"+monthArr[monthvalue-1]+" does not have "+dayvalue+" days!")
                            f.elements[thisday].focus();
                            return false
                        }
                } else  {
                if (monthvalue==4 || monthvalue==6 || monthvalue==9 || monthvalue==11)
                {
                          if (dayvalue>30)
                          {
                            alert (valid_e+"\n"+monthArr[monthvalue-1]+" does not have "+dayvalue+" days!")
                            f.elements[thisday].focus();
                            return false;
                          }
                }
                else
                {
                        if (monthvalue==2)
                        {
                                if ((dayvalue>29) || ((dayvalue>28) && (yearmod!=0)))
                                {
                                        alert (valid_e+"\n"+monthArr[monthvalue-1]+" does not have "+dayvalue+" days!")
                                        f.elements[thisday].focus();
                                        return false;
                                }
                        }
                }
                }
        }
        return true;
     }
     ';
  }


  function self_get($val,$which, &$count) {
    $str = "";
    $val=trim($val);
    if(!empty($val)) {
        $this->value=$val;
        $val_arr=explode("-",$val);
        $this->day=$val_arr[0];
        $this->month=$val_arr[1];
        $this->year=$val_arr[2];
    }

    $i=$this->year_from;
    $inc=$this->year_from;
    if(!($this->yearv <= $this->year_to && $this->yearv >= $this->year_from)) {
       $i=$this->year_from;$inc=$this->year_from;
       $this->year_arr[$i-$inc]=array("label"=>$this->yearv,"value"=>$this->yearv);
       $i=$this->year_from;$inc=$this->year_from-1;
    }
    while($i <= $this->year_to) {
     $this->year_arr[$i-$inc]=array("label"=>"$i","value"=>"$i");
     $i++;
    }


    $i=1;$inc=1;
    if(($this->monthv > 0 && $this->monthv < 13)) {

    } else {
          $this->month_arr[$i-$inc]=array("label"=>$this->monthv,"value"=>$this->monthv);
          $i=1;$inc=0;
    }

   while (list($k,$o) = each($this->month_rarr[$this->monthFormat])) {
        //if($o > 0 ) {
          $this->month_arr[$i-$inc]=array("label"=>"$o","value"=>$k+1);
        //}
        $i++;
    }

    $i=1;$inc=1;
    if(!($this->dayv > 0 && $this->dayv < 32)) {

       $i=0;$inc=0;
       $this->day_arr[$i-$inc]=array("label"=>$this->dayv,"value"=>$this->dayv);
       $i=1;$inc=0;
    }
    while($i < 32) {
     $this->day_arr[$i-$inc]=array("label"=>"$i","value"=>"$i");
     $i++;
    }


//    echo "<br>dayPos:".$this->dayPos;
//    echo "<br>monthPos:".$this->monthPos;
//    echo "<br>yearPos:".$this->yearPos;

    $ii=0;
    while($ii < 3) {
    $t="select";
    if($ii==$this->dayPos) {
        $n=$this->name."_day";
        $nvalue=$this->day;
        $this->options=$this->day_arr;
    } else if($ii==$this->monthPos) {
        $n=$this->name."_month";
        $nvalue=$this->month;
        $this->options=$this->month_arr;
    } else if($ii==$this->yearPos) {
        $n=$this->name."_year";
        $nvalue=$this->year;
        $this->options=$this->year_arr;
    }

    $str .= "<$t name='$n'";
    if ($this->size)
      $str .= " size='$this->size'";
    if ($this->extrahtml)
      $str .= " $this->extrahtml";
    $str .= ">";
    reset($this->options);
    while (list($k,$o) = each($this->options)) {
      $str .= "<option";
      if (is_array($o))
        $str .= " value='" .  $o["value"] . "'";
      if (!$this->multiple && ($nvalue==$o["value"] || $nvalue==$o))
        $str .= " selected";
      $str .= ">" . (is_array($o) ? $o["label"] : $o) . "\n";
    }
    $ii++;
    $str .= "</select>";
    }
    $count = 3;
    return $str;
  }

  function self_get_frozen($val,$which, &$count) {
    $str = "";
    $x = 3;
    $n = $this->name . ($this->multiple ? "[]" : "");

    $str="<table border=1>\n";

    if(!empty($val)) {
        $val_arr=explode("-",$val);
        $tmp_day=$val_arr[0];
        $tmp_month=$val_arr[1];
        $tmp_year=$val_arr[2];
    }

    $str.="<input type='hidden' name='".$n."_day' value='$tmp_day'>";
    $str.="<input type='hidden' name='".$n."_month' value='$tmp_month'>";
    $str.="<input type='hidden' name='".$n."_year' value='$tmp_year'>";
    $str.="<tr><td>$tmp_day-".$this->month_larr[($tmp_month-1)]."-$tmp_year</td></tr>";
    $str.= "</table>\n";
    $count = $x;
    return $str;
  }

  function self_get_js($ndx_array) {
    return " if(!checkCalendar(f,\"$this->name\",$this->year_from,$this->year_to,\"$this->comp_e\",\"$this->valid_e\")) return false;\n"; /*** call function with arguments ***/
  }

  function self_validate($val) {

  if (!$this->multiple) {
    $new_arr=explode("-",$val);
    $this->day=$new_arr[0];
    $this->month=$new_arr[1];
    $this->year=$new_arr[2];

    if($this->day < 32 && $this->day > 0) {
      $dayname=$this->day;
      $dayvalue=$this->day;
    } else {
      $dayvalue="-1";
    }

    if($this->month < 13 && $this->month > 0) {
         $monthvalue=$this->month;
         $monthname=$this->month_larr[$monthvalue-1];
    } else {
         $monthvalue="-1";
    }

    if($this->year <= $this->year_to && $this->year >= $this->year_from) {
      $yearvalue=$this->year;
    } else {
        $yearvalue="-1";
    }
    $yearmod=$yearvalue%4;

    if($this->comp_e) {
       if($dayvalue==-1 || $monthvalue==-1 || $yearvalue==-1) {
          $str=$this->comp_e;
         } else {
           $str="";
         }
            return $str;
    }
        if ($monthvalue==1 || $monthvalue==3 || $monthvalue==5 || $monthvalue==7 || $monthvalue==8 || $monthvalue==10 || $monthvalue==12) {
                if ($dayvalue>31) {
                 $ret ="$monthname does not have $dayvalue days!";
                }
        } else  {
          if ($monthvalue==4 || $monthvalue==6 || $monthvalue==9 || $monthvalue==11) {
               if ($dayvalue>30) {
                  $ret="$monthname does not have $dayvalue days!";
               }
          } else {
             if ($monthvalue==2) {
                if (($dayvalue>29) || (($dayvalue>28) && ($yearmod!=0))) {
                   $ret="$monthname does not have $dayvalue days!";
                }
             }
          }
        }
    }
 if(isset($ret)) {
   return "$ret<br>".$this->valid_e;
  }
 }



   function marshal_dispatch($m,$func) {
    $vname = $this->name;
    global $$vname;
    if($this->type=="of_calendar") {
        $kk_day=$this->name."_day";
        $kk_month=$this->name."_month";
        $kk_year=$this->name."_year";
        //global $$kk_day,$$kk_month,$$kk_year;
        //$$vname=$$kk_day."-".$$kk_month."-".$$kk_year;
            $$vname=$_POST["$kk_day"]."-".$_POST["$kk_month"]."-".$_POST["$kk_year"];
    }
    return $this->$func($$vname);
  }


} // end of_calendar
?>
