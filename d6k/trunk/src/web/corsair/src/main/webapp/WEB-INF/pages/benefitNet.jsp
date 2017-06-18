<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/12/24
  Time: 10:37
  To change this template use File | Settings | File Templates.
--%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>

<!DOCTYPE html>
<html lang="zh_CN">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">
  <title>综合效益表</title>
  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
  <style >
    td{text-align: center}
  </style>
</head>
<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
<script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>

<script src="resources/dist/laydate/laydate.js"></script>
<script type="application/javascript">
  $(function(){
     $("tbody tr:even").attr("class","tr-even");
      $("tbody tr:odd").attr("class","tr-odd");
      /*$(".tr-even").css("background-color","rgb(31, 86, 134)");*/
      $(".tr-odd").children().css("color","white");
      $(".gray-bg").css("color","black");
      $(".Electric-color").css("color","black");
  });

</script>

<script type="text/javascript">
  function searchElectricity(){
    with(document.forms[0]){
    }
    document.getElementById('submit').click();
    return true;
  }
</script>
<script type="text/javascript" language="javascript">
  var idTmr;
  function  getExplorer() {
    var explorer = window.navigator.userAgent ;
    //ie
    if (explorer.indexOf("MSIE") >= 0) {
      return 'ie';
    }
    //firefox
    else if (explorer.indexOf("Firefox") >= 0) {
      return 'Firefox';
    }
    //Chrome
    else if(explorer.indexOf("Chrome") >= 0){
      return 'Chrome';
    }
    //Opera
    else if(explorer.indexOf("Opera") >= 0){
      return 'Opera';
    }
    //Safari
    else if(explorer.indexOf("Safari") >= 0){
      return 'Safari';
    }
  }
  function method1(tableid) {//整个表格拷贝到EXCEL中
    if(getExplorer()=='ie')
    {
      var curTbl = document.getElementById(tableid);
      //创建AX对象excel
      var oXL = new ActiveXObject("Excel.Application");
      //获取workbook对象
      var oWB = oXL.Workbooks.Add();
      //激活当前sheet
      var xlsheet = oWB.Worksheets(1);
      //把表格中的内容移到TextRange中
      var sel = document.body.createTextRange();
      sel.moveToElementText(curTbl);
      //全选TextRange中内容
      sel.select();
      //复制TextRange中内容
      sel.execCommand("Copy");
      //粘贴到活动的EXCEL中
      xlsheet.Paste();
      //设置excel可见属性
      oXL.Visible = true;

      try {
        var fname = oXL.Application.GetSaveAsFilename("Excel.xls", "Excel Spreadsheets (*.xls), *.xls");
      } catch (e) {
        print("Nested catch caught " + e);
      } finally {
        oWB.SaveAs(fname);

        oWB.Close(savechanges = false);
        //xls.visible = false;
        oXL.Quit();
        oXL = null;
        //结束excel进程，退出完成
        //window.setInterval("Cleanup();",1);
        idTmr = window.setInterval("Cleanup();", 1);
      }
    }
    else
    {
      tableToExcel(tableid)
    }
  }
  function Cleanup() {
    window.clearInterval(idTmr);
    CollectGarbage();
  }
  var tableToExcel = (function() {
    var uri = 'data:application/vnd.ms-excel;base64,',
            template = '<html xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:x="urn:schemas-microsoft-com:office:excel" xmlns="http://www.w3.org/TR/REC-html40"><head><!--[if gte mso 9]><xml><x:ExcelWorkbook><x:ExcelWorksheets><x:ExcelWorksheet><x:Name>{worksheet}</x:Name><x:WorksheetOptions><x:DisplayGridlines/></x:WorksheetOptions></x:ExcelWorksheet></x:ExcelWorksheets></x:ExcelWorkbook></xml><![endif]--></head><body><table>{table}</table></body></html>',
            base64 = function(s) { return window.btoa(unescape(encodeURIComponent(s))) },
            format = function(s, c) {
              return s.replace(/{(\w+)}/g,
                      function(m, p) { return c[p]; }) }
    return function(table, name) {
      if (!table.nodeType) table = document.getElementById(table)
      var ctx = {worksheet: name || 'Worksheet', table: table.innerHTML}
      window.location.href = uri + base64(format(template, ctx))
    }
  })()
</script>

<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>
  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-12">
        <h2 class="page-header">入网电费结算</h2>
      </div>
    </div>
    <div class="row">
      <div style="text-align:left; padding-bottom:5px; width: 80%">
        <form:form cssClass="form-horizontal" action="benefitNet" method="post" commandName="benefitNetForm">
          <form:hidden property="printDate" path="printDate"/>
          <div class="form-group">
            <label>选择站点</label>
            <form:select path="stationId" cssClass="form-control layer-date">
              <form:options items="${stationList}" itemValue="id" itemLabel="name"/>
            </form:select>
            <label class="m-l-md">日期</label>
            <form:input cssClass="form-control laydate-icon layer-date" id="selectDateId" property="selectDate" path="selectDate" />
            <script>
              var reportColumns = [];
              var columndatas = [];
              var reportTable = [];
              var dataConfig = {
                elem: '#selectDateId',
                format: 'YYYY/MM',
                max: '2099-06-16',
                istime: false,
                istoday: false,
                choose: function(dates){ //选择好日期的回调
//                    $.post('/dayReportServlet',{date:dates},function (response) {
//                      reportColumns = response.FieldName;
//                      columndatas = response.data;
//                    },'json');
                }
              }
              laydate(dataConfig);
            </script>
            <form:input type="submit" cssClass="btn btn-primary"  path="submit" value="查询" onclick="return searchElectricity();"/>
            <input type="button" class="btn btn-primary m-l-sm" onclick="javascript:method1('targetTable')" value="导出">
          </div>
        </form:form>
      </div>

      <div class="row">
        <div class="col-lg-12 col-md-12">
          <div class="table-responsive">
            <table id="targetTable" class="table table-bordered table-striped" style="text-align: center">
              <tr style="text-align: center">
                <td colspan="6" class="gray-bg"><h4><strong>${initDate}${stationName}入网结算单</strong></h4></td>
              </tr>
              <tr>
                <td>抄表日期</td>
                <td colspan="2">${initDate}</td>
                <td >打印日期</td>
                <td colspan="2">${printDate}</td>
              </tr>
              <tr>
                <td>户名</td>
                <td colspan="2">${electricStationInfo.name}</td>
                <td>电话号码</td>
                <td colspan="2">${electricStationInfo.phoneNo}</td>
              </tr>
              <tr>
                <td>户号</td>
                <td colspan="2">${electricStationInfo.comments}</td>
                <td>地址</td>
                <td colspan="2">${electricStationInfo.address}</td>
              </tr>
              <tr style="text-align: center">
                <td colspan="6" class="gray-bg"><h4><strong>高配上网电量总计量</strong></h4></td>
              </tr>
              <tr>
                <td class="table-head"></td>
                <td>电表编码</td>
                <td>本月示数</td>
                <td>上月示数</td>
                <td>倍数</td>
                <td>电量(kWh)</td>
              </tr>
              <c:forEach items="${netElectricList}" var="ElectricPSR" varStatus="i">
                <tr>
                  <c:if test="${i.count == 1}">
                    <td rowspan="${fn:length(netElectricList)}" style="vertical-align:middle">高配计量表</td>
                  </c:if>
                  <td>${ElectricPSR.getName()}</td>
                  <td>${ElectricPSR.getCurValue()}</td>
                  <td>${ElectricPSR.getLastValue()}</td>
                  <td>${ElectricPSR.getMultiple()}</td>
                  <td>${ElectricPSR.getElectricValue()}</td>
                </tr>
              </c:forEach>

              <tr style="text-align: center">
                <td colspan="6" class="gray-bg"><h4><strong>上网电量电费结算</strong></h4></td>
              </tr>
              <tr>
                <td class="table-head"></td>
                <td>电表编码</td>
                <td>电量(kWh)</td>
                <td colspan="2">结算电价（元/kWh）</td>
                <td>电费（元）</td>
              </tr>
              <c:forEach items="${netElectricAccount}" var="ElectricPSR" varStatus="j">
                <tr>
                  <c:if test="${j.count == 1}">
                    <td rowspan="${fn:length(netElectricAccount)+2}" style="vertical-align:middle" class="table-head Electric-color">上网电量结算</td>
                  </c:if>
                  <td>${ElectricPSR.getName()}</td>
                  <td>${ElectricPSR.getElectricValue()}</td>
                  <td colspan="2">${ElectricPSR.getElectricPrice()}</td>
                  <td>${ElectricPSR.getFee()}</td>
                </tr>
              </c:forEach>
              <tr>
                <td class="strongfont"><strong>合计金额</strong></td>
                <td colspan="4" ><strong>${totalNetElectricFee}</strong></td>
              </tr>
              <tr>
                <td><strong>合计大写金额</strong></td>
                <td colspan="4"><strong>${totalFeeUppercase}</strong></td>
              </tr>
            </table>
          </div>
        </div>
      </div>
    </div>
  <%--</div>
</div>--%>
</body>
</html>

