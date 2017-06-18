<%--
  Created by IntelliJ IDEA.
  User: ChengXi
  Date: 2015/9/7
  Time: 9:53
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html lang="zh_CN">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <link href="resources/css/bootstrap.min.css" rel="stylesheet">
  <link href="resources/css/metisMenu.min.css" rel="stylesheet">
  <link href="resources/css/sb-admin-2.css" rel="stylesheet">
  <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">



  <script src="resources/dist/laydate/laydate.js"></script>

  <link rel="stylesheet" type="text/css" href="resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>
  <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/WdatePicker.css"/>
  <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/default/datepicker.css"/>
  <link href="resources/css/style.css" rel="stylesheet" type="text/css">
</head>

<body>

<script src="resources/js/jquery.min.js"></script>
<script src="resources/js/bootstrap.min.js"></script>
<script src="resources/js/metisMenu.js"></script>
<script src="resources/js/sb-admin-2.js"></script>
<script type="text/javascript" src="resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"></script>
<script type="text/javascript" src="resources/bower_components/dtGrid/i18n/zh-cn.js"></script>
<script type="text/javascript" src="resources/bower_components/datePicker/WdatePicker.js" defer="defer"></script>

<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>

    <div id="page-wrapper">--%>

      <div class="row">
        <div class="col-lg-12">
          <div class="panel">
            <div class="panel-body">
              <div class='col-md-6'>
                <div class='input-group' style="margin: 15px 0px;">
                  <span class='input-group-addon'>年份</span>
                  <input class='laydate-icon layer-date' id="ReportTableDateId">
                  <script>

                    var reportColumns = [];
                    var columndatas = [];
                    var reportTable = [];
                    var dataConfig = {
                      elem: '#ReportTableDateId',
                      format: 'YYYY',
                      max: '2099-06-16',
                      istime: false,
                      istoday: false,
                      choose: function(dates){ //选择好日期的回调
                        $.post('/api/yearReport',{date:dates},function (response) {
                          reportColumns = response.FieldName;
                          columndatas = response.data;
                        },'json');
                      }
                    }
                    laydate(dataConfig);
                  </script>
                  <button class="btn btn-primary" onclick="displayData()"><i class="fa fa-search"></i>查询</button>
                </div>

              </div>
              <div class="col-md-12" id = "reportTable"></div>
              <div class="col-md-12" id = "reportTableToolBar"></div>
              <script>
                function displayData(){

                  var reportOption = {
                    id:'reportTable',
                    lang: 'zh-cn',
                    ajaxLoad: false,
                    datas: columndatas,
                    columns: reportColumns,
                    gridContainer: 'reportTable',
                    toolbarContainer: 'reportTableToolBar',
                    tools : '',
                    pageSize: 20,
                    pageSizeLimit: [10, 20, 50],
                    onGridComplete : function(grid){
                      $("tbody tr:even").children().css("color","white");
                      $("tbody tr:even").css("background","rgb(31, 86, 134)");
                      for(var i = 1 ;i<=grid.option.datas.length;i++){
                        var num = 4*i-2;
                        $("tbody tr").eq(num).css("background","#82accb");
                        $("tbody tr").eq(num).children().css("color","black");
                      }
                    }
                  };

                  reportTable = $.fn.DtGrid.init(reportOption);
                  reportTable.load();
                }

                function init(){
                  var date = new Date();

                  var year = date.getFullYear();

                  var now = year;

                  document.getElementById("ReportTableDateId").value = now;
                  $.post('/api/yearReport',{date:now},function (response) {
                    reportColumns = response.FieldName;
                    columndatas = response.data;
                    displayData();
                  },'json');

                }

                window.onload=function(){init();}
              </script>
            </div>

          </div>
        </div>
      </div>
<%--</div>
</div>--%>
</body>
</html>
