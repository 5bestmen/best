<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
  <title>${sessionScope.APP_NAME}-实时数据查询</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/multiSelect/sol.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/alertify.min.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/multiSelect/bootstrap-multiselect.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sweetalert.min.css" />" rel="stylesheet">

  <style type="text/css">
    ul {
      list-style: none;
      margin: 0px;
      padding: 0;
    }

    .label {
      color: black;
      text-align: left;
      font-size: 14px;
    }

    .sol-label-text {
      color: #ffffff;
    }

    /*.btn{
        background-color: #ffffff;
        color: #000000;
    }*/
    .multiselect-clear-filter {
      padding: 9px 12px;
    }

    .query {
      margin-top: 20px;
    }

    #mytable td {
      color: #ffffff;
      padding-bottom: 10px;
    }

    .form-group {
      margin: 5px 0px;
    }

    .tab-container { margin-top: 50px; }

    .etabs { margin: 0; padding: 0; margin-left:10px;}

    .etabs1 { margin: 0; padding: 0; margin-left:10px;}

    .etabs2 { margin: 0; padding: 0; margin-left:10px;}

    .tab { font-size:12px; display: inline-block; zoom:1; *display:inline; background: #6c0; border:1px solid #FFFFE0; border-bottom: none; -moz-border-radius: 6px 6px 0 0; -webkit-border-radius: 6px 6px 0 0; }

    #conai{/* padding: 35px; */ width:auto; float:left; /* margin-top:-15px; */}

    .tabMain{padding-left: 8px; width: 100%; background-color: #FFFFE0; padding-right: 8px; bordercolor:#FF0000; cellpadding:0; cellspacing:0;}
    /*table width="99%" border="1" cellpadding="0" cellspacing="0" bordercolor="#FF0000" >*/

    .tabTopBar{height: 40px; line-height: 40px; padding-left: 8px; background-color: #47a6e0; width:100%;}

    .ttbTxt{width: auto; color: #fff; font-size: 14px; font-weight: bold; text-align:center}

    .test1{text-align:center; color:#6c0; font-size:18px}

    .panel-container { background: #fff; padding: 10px; -moz-border-radius: 0 4px 4px 4px; -webkit-border-radius: 0 4px 4px 4px; }

    .col-xs-3 {
      padding-left: 0px;
      width: 22%;
    }

    .col-xs-4 {
      width: 32%;
    }

    .multiselect {
      background-color: #ffffff;
      color: #555;
    }

    .laydate_body .laydate_table td {
      color: black;
    }

    .btn-group {
      float: right;
    }

    .laydate-icon {
      background-image: url(/resources/img/Calendar.png);
    }

    /*.dropdown-menu, .dropdown-menu > li > a:hover, .dropdown-menu > li > a {*/
    /*background-color: rgb(31, 86, 134);*/
    /*}*/
  </style>


</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/toc.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/sol.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/bootstrap-multiselect.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/alertify.min.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script>
  $(function () {
    var meaObject = $.fn.GeneralMeasurement.init();
    meaObject.load();
  });
</script>

<script>
  Date.prototype.format = function (fmt) {
    var o = {
      "M+": this.getMonth() + 1, //月份
      "d+": this.getDate(), //日
      "h+": this.getHours(), //小时
      "m+": this.getMinutes(), //分
      "s+": this.getSeconds(), //秒
      "q+": Math.floor((this.getMonth() + 3) / 3), //季度
      "S": this.getMilliseconds() //毫秒
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
      if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
  };

  function timeInit() {
    var startTime, endTime;
    startTime = laydate.now(0, "YYYY-MM-DD 06");
    endTime = laydate.now(0, "YYYY-MM-DD 18");
    $('#queryStartTime').val(startTime);
    $('#queryEndTime').val(endTime);
  }

  function styleInit() {
    $("select[name=example-selectAllJustVisible]").multiselect({
      buttonWidth: '100%',
      buttonText: function (options) {
        if (options.length == 0) {
          return '无选择';
        }
      }
    });
  }

  var count = 0;
  function search() {

    var selectedOptions = $('td[class="col-xs-5"]').find('option:selected');
    if (selectedOptions[0].innerHTML === '请选择' || selectedOptions[1].innerHTML === '请选择') {
      swal({
        title: "请选择，再查询！",
        text: "请选择，再查询！",
        confirmButtonText: "确定"
      });
      return;
    }
    var stnID = selectedOptions[0].value;
    var devID = selectedOptions[1].value;

    var stationId =  $(this).attr("stationId");
    var deviceId =  $(this).attr("deviceId");



    show(stnID, devID);

    //show('dd');

    //show('di');
  }

  function init(obj) {
    $.ajax({
      type: "GET",
      url: "/api/psr/subStations",
      contentType: "application/json",
      dataType: "json",
      async: false,
      success: function (data) {
        result = data;
        for (var i = 0; i < result.length; i++) {
          obj.append("<option value='" + result[i].id + "'>" + result [i].name + "</option>");
        }
      }
    });
  }

  function deleteTr() {
    $("button[name=delete]").click(function (event) {
      count--;
      $(event.target).parent().parent().remove();
    })
  }

  function findParentElement(element) {
    return $(element).parent();
  }

  function findNextElement(element) {
    return $(element).parent().next().next().children();
  }

  function showEquipments() {
    $('select[name=station]').change(function (event) {
      var $select = findParentElement(event.target);
      var $nextElement = findNextElement(event.target);
      var stationId = $select.find("option:selected").val();
      $.ajax({
        type: "GET",
        url: '/api/psr/subStation/' + stationId + '/devices',
        contentType: "application/json",
        dataType: "json",
        async: false,
        success: function (data) {
          result = data;
          $nextElement.html("");
          $nextElement.append("<option>请选择</option>");
          for (var i = 0; i < result.length; i++) {
            $nextElement.append("<option value='" + result[i].id + "' stationId='" + stationId + "' deviceId='" + result[i].id + "'>" +
            result [i].name + "</option>");
          }
        }
      });
    });
  }

  function showDevices() {
    $('select[name=device]').change(function (event) {
      var select = findParentElement(event.target);
      var nextElement = findNextElement(event.target);
      nextElement.find("option:selected").each(function () {
        $(this).attr("selected", false);
      })
      var deviceId = select.find("option:selected").val(),
              stationName = select.parent().children().eq(1).find("option:selected").text(),
              deviceName = select.find("option:selected").text();
      /*$.ajax({
        type: "GET",
        url: '/api/psr/devices/' + deviceId + '/templates/curves',
        contentType: "application/json",
        dataType: "json",
        async: false,
        success: function (data) {
          result = data;
          for (var i = 0; i < result.length; i++) {
            nextElement.attr("multiple", "multiple");
            nextElement.append("<option value='" + result[i].id + "' name='" + result[i].name + "' stationName='" + stationName + "' deviceName='" + deviceName + "' id='" + deviceId + "' unit='" + result[i].chartUnit + "'>" + result [i].desc + '(' + result[i].unit + ')' + "</option>");
          }
        }
      });
      getAllData(nextElement);*/
    });
  }


  function deleteRepeatElement(element) {
    //去掉数组里面重复才元素，保证新数组每个元素的唯一性
    var selectedOption = $('td[class="col-xs-7"]').find('option:selected');
    var new_arr = [];
    getNotRepeatingUnit(new_arr, selectedOption);
    if (new_arr.length == 3) {
      element.attr("selected", false);
      var input = $('input[value="' + element.val() + '"]');
      input.parent().parent().parent().removeClass();
      input.attr("checked", false);
      swal({
        title: "目前只能支持最多两种单位！",
        text: "目前只能支持最多两种单位.请重新选择",
        confirmButtonText: "确定"
      });
    }
  }

  function isNotRepeat(content) {
    var templates = $('td[class="col-xs-7"]').find('option:selected');
    var template = [];
    var data = content.attr("id") + content.text();
    templates.each(function () {
      template.push($(this).attr("id") + $(this).text());
    });
    template.splice($.inArray(data, template), 1);
    for (var k = 0; k < template.length; k++) {
      if (template[k] == data) {
        content.attr("selected", false);
        var input = $('input[value="' + content.val() + '"]');
        input.parent().parent().parent().removeClass();
        input.attr("checked", false);
        swal({
          title: "已经勾选过，请选择其他选项！",
          text: "已经勾选过，请选择其他选项！",
          confirmButtonText: "确定"
        });
        break;
      }
    }
  }

  function getNotRepeatingUnit(new_arr, templates) {
    for (var i = 0; i < templates.length; i++) {
      var items = templates.eq(i).attr("unit");
      if ($.inArray(items, new_arr) == -1) {
        new_arr.push(items);
      }
    }
  }

  function getAllData(obj) {
    obj.multiselect({
      enableFiltering: true,
      filterPlaceholder: '查找',
      buttonWidth: '100%',
      enableHTML: true,
      buttonContainer: '<div class="btn-group" />',
      maxHeight: false,
      buttonText: function (options, select) {
        var templates = $('td[class="col-xs-7"]').find('option:selected');
        var template = [];
        var data = options.eq(options.length - 1).attr("id") + options.eq(options.length - 1).text();
        templates.each(function () {
          template.push($(this).attr("id") + $(this).text());
        });
        template.splice($.inArray(data, template), 1);
        for (var k = 0; k < template.length; k++) {
          if (template[k] == data) {
            var currentOptions = [];
            return getOption(currentOptions, options);
            break;
          }
        }

        var new_arr = [];
        getNotRepeatingUnit(new_arr, templates);
        if (new_arr.length == 3) {
          var newOptions = [];
          return getOption(newOptions, options);
        } else {
          if (options.length == 0) {
            return '请选择';
          } else {
            var selected = '';
            options.each(function () {
              selected += $(this).text() + ', ';
            });
            return selected.substr(0, selected.length - 2);
          }
        }

      },
      onChange: function (element, checked) {
        if (checked) {
          deleteRepeatElement(element);
          isNotRepeat(element);
        }
        // Get selected options.只能选择5个选项
        var selectedOptions = obj.find('option:selected')
        if (selectedOptions.length >= 5) {
          // Disable all other checkboxes.
          var nonSelectedOptions = obj.eq(1).find('option').filter(function () {
            return !$(this).is(':selected');
          });
          nonSelectedOptions.each(function () {
            var input = $('input[value="' + $(this).val() + '"]');
            input.prop('disabled', true);
            input.parent('li').addClass('disabled');
          });
        }
        else {
          // Enable all checkboxes.
          obj.eq(1).find('option').each(function () {
            var input = $('input[value="' + $(this).val() + '"]');
            input.prop('disabled', false);
            input.parent('li').addClass('disabled');
          });
        }


      }
    });
  }

  function getOption(newOptions, options) {
    for (var i = 0; i < options.length - 1; i++) {
      newOptions.push(options.get(i));
    }
    if (newOptions.length == 0) {
      return '请选择';
    } else {
      var selected = '';
      for (var j = 0; j < newOptions.length; j++) {
        selected += newOptions[j].text + ', ';
      }
      return selected.substr(0, selected.length - 2);
    }
  }

  $(function () {
    $('[data-toggle="tooltip"]').tooltip();

    init($("select[name=station]"));
    showEquipments();
    showDevices();
    styleInit();
    timeInit();

    var timeStart = {
      elem: '#queryStartTime',
      format: 'YYYY-MM-DD hh',
      istime: true,
      istoday: true
    };

    var timeEnd = {
      elem: '#queryEndTime',
      format: 'YYYY-MM-DD hh',
      istime: true,
      istoday: true
    };

    laydate(timeStart);
    laydate(timeEnd);

    $('#today').on('click', function () {
      timeInit();
    });

    $('#prev').on('click', function () {
      var endTime = new Date(Date.parse($('#queryEndTime').val().replace(/-/g, "/") + ":00:00"));
      endTime.setDate(endTime.getDate() - 1);
      $('#queryEndTime').val(endTime.format("yyyy-MM-dd") + ' 18');
      $('#queryStartTime').val(endTime.format("yyyy-MM-dd") + ' 06');
    });

    $('#next').on('click', function () {
      var endTime = new Date(Date.parse($('#queryEndTime').val().replace(/-/g, "/") + ":00:00"));
      var today = new Date();

      if (endTime.format("yyyy-MM-dd") == today.format("yyyy-MM-dd")) {
        swal({
          title: "不能查询未来的数据!",
          text: "不能查询未来的数据!！",
          confirmButtonText: "确定"
        });
        return;
      }

      endTime.setDate(endTime.getDate() + 1);
      $('#queryEndTime').val(endTime.format("yyyy-MM-dd") + ' 18');
      $('#queryStartTime').val(endTime.format("yyyy-MM-dd") + ' 06');
    });


  });

  /*var timer;

  $(function(){
    $('#tab-container').easytabs({defaultTab:'li#tab1'});

    // 表格hover事件绑定
    $(".tabMain").find("tr").hover(
            function(){
              $(this).find("td").css({
                "background-color":"#eff4f7"
              });
            },
            function(){
              $(this).find("td").css({
                "background-color":""
              });
            }
    );

   $("#a1").click();
    refr();
    //timer = setInterval(refr,${timeout});

    var typeId = '${typeId.value}';
    if(typeId!=''){
      $("#typeId").val(typeId);
    }
  });*/

   /*function refr() {
     var refId = $("#refId").val();
     var tableObj = $('#' + refId + '_table tbody');
     tableObj.empty();
     for (var i = 0; i < 10; i++) {
       var tr = "<tr>";
       // 一期故障解列装置INT-PA620-A1
       tr += "<tr> <td style=\"text-align:center; color:#6c0;\">" + "一期故障解列装置" + "</td>";
       tr += "<td style=\"text-align:center; color:#6c0;\">" + "1111.11" + "</td>";
       tr += "<td style=\"text-align:center; color:#6c0;\">" + "2016.11.30 15:02" + "</td> </tr>";
       tr += "</tr>";
       tableObj.append(tr);
     }
   }*/

  /*
   $.ajax({
   type: "POST",
   url: "/api/multiCharts/dayReport",
   contentType: "application/json",
   dataType: "json",
   data: JSON.stringify(data),
   success: function (result) {
   var eOption = {
   legend: {
   padding: 25
   },
   grid: {
   y: '100px'
   }
   };

   myChart = echarts.init(document.getElementById('powerCurve'), "macarons");
   myChart.setOption(eOption);
   var options = myChart.getOption();
   options = result;
   myChart.hideLoading();
   myChart.setOption(options);
   },
   error: function (errorMsg) {
   swal({
   title: "不好意思，图表请求数据失败啦!",
   text: "不好意思，图表请求数据失败啦!",
   confirmButtonText: "确定"
   });
   }
   });
   */
  function refr(stnId, devId){
    //var refId = $("#refId").val();
    $.ajax({
      type : "POST",
      url: "/api/realtimeCharts/rdbData",
      async : false,
      data : {
        "stationId": stnId,
        "deviceId": devId

    //"stationId": "1",
        //"deviceId": "1"
      },
      success : function(result)
      {
        /*var dataObj = jQuery.parseJSON(result);

        var tableObj = $('#'+refId+'_table tbody');
        tableObj.empty();
        var dataList = dataObj.modelMap.dataList;
        for(var i=0;i<dataList.length;i++){
          var tr = "<tr>";
          tr += "<tr> <td style=\"text-align:left\">"+dataList[i].describe+"</td>";
          tr += "<td style=\"text-align:right\">"+dataList[i].value+"</td>";
          tr += "<td style=\"text-align:center\">"+dataList[i].timeOfValue+"</td> </tr>";
          tr += "</tr>";
          tableObj.append(tr);
        }*/

        //var refId = $("#refId").val();
        //var tableObj = $('#' + refId + '_table tbody');
        //tableObj.empty();

        var aiTableID = $('#ai_table tbody');
        var diTableID = $('#di_table tbody');
        var calcTableID = $('#calc_table tbody');
        aiTableID.empty();
        diTableID.empty();
        calcTableID.empty();

        for (var i = 0; i < result.aiList.length; i++) {
          var tr = "<tr>";
          tr += "<tr> <td style=\"text-align:center; color:#ffff00; font-size:15px\">" + result.aiList[i].name + "</td>";
          tr += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result.aiList[i].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
          tr += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result.aiList[i].unit + "</td> </tr>";
          tr += "</tr>";
          aiTableID.append(tr);
        }

        for (var i = 0; i < result.diList.length; i++) {
          var tr = "<tr>";
          tr += "<tr> <td style=\"text-align:center; color:#ffff00; font-size:15px\">" + result.diList[i].name + "</td>";
          tr += "<td class=\"measurement\" data-measurementType=\"1\"" + " data-measurementID=" + result.diList[i].code + " style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
          tr += "</tr></tr>";
          diTableID.append(tr);
        }

        for (var i = 0; i < result.calcList.length; i++) {
          var tr = "<tr>";
          tr += "<tr> <td style=\"text-align:center; color:#ffff00; font-size:15px\">" + result.calcList[i].name + "</td>";
          tr += "<td class=\"measurement\" data-measurementType=\"3\"" + " data-measurementID=" + result.calcList[i].code + " style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
          tr += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result.calcList[i].unit + "</td> </tr>";
          tr += "</tr>";
          calcTableID.append(tr);
        }

        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();

      }
    });
  }

  function show(stnId, devId){
    //$("#refId").val(refId);
    refr(stnId, devId);
  }



</script>

<div id="content" class="clear">

  <form class="form-horizontal">
    <div class="col-md-12 query">
      <table id="mytable">
        <tr>
          <td>厂站</td>
          <td class="col-xs-5">
            <select name="station" class="form-control">
              <option>请选择</option>
            </select>
          </td>
          <td>设备</td>
          <td class="col-xs-5">
            <select name="device" class="form-control">
              <option>请选择</option>
            </select>
          </td>
          <!--td>测点</td>
          <td class="col-xs-7" style="width: 53%"><select name="example-selectAllJustVisible"
                                                          multiple="multiple">
          </select></td-->
          <%--<td>--%>
            <%--<input class="checkbox" name="auto" type="checkbox" value=" auto">auto--%>
          <%--</td>--%>
          <td>
            <button type="button" class="btn  btn-success btn-sm" onclick="search()">查询</button>
              <%--<button type="button"  class="btn btn-success" id="query2">查询</button>--%>
          </td>
        </tr>
      </table>
    </div>
  </form>
  </br>

  <div id="tab-container" class='tab-container'>
    <input id="currentTab" name="currentTab" type="hidden" value="${tabName }" />
    <input id="tabId" name="tabId" type="hidden" value="${tabId }" />
    <input id="refId" name="refId" type="hidden" value="dd" />

      <div id="conai" style="float: left">
        <div class="tabTopBar">
          <div class="ttbTxt">遥测数据列表</div>
        </div>
        <table id="ai_table" border="1" bordercolor='grey' cellpadding="2" cellspacing="1">
          <thead>
          <tr>
            <th style="text-align:center; color: #d3d3d3;" width="150px; ">描述</th>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">值</th>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">单位</th>
          </tr>
          </thead>
          <tbody>

          <!--tr>
            <tr> <td style="text-align:center; color:#6c0; font-size:18px">"一期故障解列装置"</td>
            <td class="measurement" data-measurementID="1-56-2-0400" style="text-align:center; color:#6c0; font-size:18px"></td>
            <td style="text-align:center; color:#6c0; font-size:18px">"2016.11.30 15:02"</td> </tr>
          </tr-->
          </tbody>
        </table>
      </div>

      <div id="condi" style="float: left">
        <div class="tabTopBar">
          <div class="ttbTxt">遥信数据列表</div>
        </div>
        <table id="di_table" border="1" bordercolor='grey' cellpadding="2" cellspacing="1">
          <thead>
          <tr>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">描述</th>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">值</th>
          </tr>
          </thead>
          <tbody>

          </tbody>
        </table>
      </div>

      <div id="condd" style="float: left">
        <div class="tabTopBar">
          <div class="ttbTxt">计算量数据列表</div>
        </div>
        <table id="calc_table" border="1" bordercolor='grey' cellpadding="2" cellspacing="1">
          <thead>
          <tr>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">描述</th>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">值</th>
            <th style="text-align:center; color: #d3d3d3;" width="150px;">单位</th>
          </tr>
          </thead>
          <tbody>

          </tbody>
        </table>
      </div>

    <div class="col-md-12 ">
      <div class="panel-body">
        <div id="powerCurve" style="height: 600px;">
        </div>
      </div>
    </div>
  </div>
</div>



</body>

</html>
