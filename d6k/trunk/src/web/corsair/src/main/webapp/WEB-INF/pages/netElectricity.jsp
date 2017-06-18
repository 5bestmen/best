<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/12/17
  Time: 9:20
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

  <title>上网电量</title>

  <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
  <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
  <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
  <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
  <script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

  <script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
  <%--<script src="<c:url value="/resources/dist/laydate/laydate.js">"></script>--%>
  <script src="resources/dist/laydate/laydate.js"></script>

  <script type="text/javascript" src="js/common.js"></script>
  <script type="text/javascript">
    function checkNetElectricity(){
      with(document.forms[0]){
        if(!netDate.value){
          alert('日期必须填写！');
          return false;
        }
        if(!totalKwh.value) {
          alert('正向有功总必须填写！');
          return false;
        }
        if(!peakKwh.value) {
          alert('正向有功峰须填写！');
          return false;
        }
        if(!normalKwh.value) {
          alert('正向有功平必须填写！');
          return false;
        }
        if (!valleyKwh.value) {
          alert('正向有功谷必须填写！');
          return false;
        }
        if (!reverseTotalKwh.value) {
          alert('反向有功总必须填写！');
          return false;
        }

        if (!totalKVar.value) {
          alert('无功总必须填写！');
          return false;
        }
        if (!multiple.value) {
          alert('电表倍数必须填写！');
          return false;
        }
      }

      isExistDate.value = false;
      if(document.getElementById('netDateError').innerText){
        if(confirm("当前选中日期已有上网电量数据，是否需要更改？"))
        {
          isExistDate.value = true;
        }
        else{
          return false;
        }
      }
        document.getElementById('submit').click();
        return true;
    }

    function checkNetDate(){
      var netDate = document.forms[0].netDate.value.trim();
      if(netDate){
        eval("var params = {netDate:'"+encodeURIComponent(netDate)+"'}");
        Ext.Ajax.request({
          method : 'POST',
          url: '/api/checkNetDate',
          success:checkNetDateAndBack,
          failure:function(){},
          params:params
        });
      }
    }

    function checkNetDateAndBack(request){
      if(request.responseText){
        var isExist = request.responseText;
        if(isExist=='1'){
          document.getElementById('netDateError').innerText='当前选中日期已有上网电量数据！';
        }
        else if(isExist=='0'){
          document.getElementById('netDateError').innerText='';
        }
      }
    }
  </script>
</head>
<body>
<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>
  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-12">
        <h2 class="page-header">上网电量</h2>
      </div>
    </div>
    <form:form cssClass="form-horizontal" action="netElectricity" method="post" commandName="netElectricityForm">
      <form:hidden path="isExistDate"/>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">余电上网日期</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control laydate-icon" id="netElectricDateId" property="netDate" path="netDate" onblur="checkNetDate()"/>
          <script>
            var reportColumns = [];
            var columndatas = [];
            var reportTable = [];
            var dataConfig = {
              elem: '#netElectricDateId',
              format: 'YYYY/MM/DD',
              max: '2099-06-16',
              istime: false,
              istoday: false,
              choose: function(dates){ //选择好日期的回调
                $.post('/dayReportServlet',{date:dates},function (response) {
                  reportColumns = response.FieldName;
                  columndatas = response.data;
                },'json');
              }
            }
            laydate(dataConfig);
          </script>
        </div>
        <span id="netDateError" style="color:red"></span>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">正向有功 总(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="totalKwh" path="totalKwh" />
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">正向有功 峰(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="peakKwh" path="peakKwh"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">正向有功 平(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="peakKwh" path="normalKwh"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">正向有功 谷(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="peakKwh" path="valleyKwh"/>
        </div>
      </div>
       <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">反向有功 总(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="reverseTotalKwh" path="reverseTotalKwh"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">无功 总(kwh)</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="peakKwh" path="totalKVar"/>
        </div>
      </div>
       <div class="form-group">
        <div class="col-sm-2 col-md-offset-2 text-right">
          <label class="control-label">电表倍数</label>
        </div>
        <div class="col-xs-3">
          <form:input cssClass="form-control" property="peakKwh" path="multiple"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-1 col-md-offset-3 text-right" >
          <label class="control-label">所属厂站</label>
        </div>
        <div class="col-xs-3">
          <form:select path="stationId" cssClass="form-control">
            <%--<form:option value="-1" label="--请选择厂站"/>--%>
            <form:options items="${stationList}" itemValue="id" itemLabel="name"/>
          </form:select>
        </div>
      </div>
      <input type="button" class="btn col-md-offset-4" value="返回" onclick="history.go(-1);" />
      <form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkNetElectricity();"/>
    </form:form>
  <%--</div>
</div>--%>
</body>
</html>

