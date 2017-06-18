<%--
  Created by IntelliJ IDEA.
  User: zjq
  Date: 2015/11/8
  Time: 18:42
  To change this template use File | Settings | File Templates.
--%>


<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <title>入库信息编辑</title>

  <link href="resources/css/bootstrap.min.css" rel="stylesheet">
  <link href="resources/css/metisMenu.min.css" rel="stylesheet">
  <link href="resources/css/sb-admin-2.css" rel="stylesheet">
  <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="resources/css/style.css" rel="stylesheet" type="text/css">


  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <script src="resources/dist/js/html5shiv.min.js"></script>
  <script src="resources/dist/js/respond.min.js"></script>
  <![endif]-->

  <script src="resources/dist/laydate/laydate.js"></script>
  <%--todo 页面自身需要的CSS begin--%>
  <link rel="stylesheet" type="text/css" href="resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>
  <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/WdatePicker.css"/>
  <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/default/datepicker.css"/>
  <%--todo 页面自身需要的CSS end--%>
</head>

<body>
<script src="resources/js/jquery.min.js"></script>
<script src="resources/js/bootstrap.min.js"></script>
<script src="resources/js/metisMenu.js"></script>
<script src="resources/js/sb-admin-2.js"></script>
<script type="text/javascript" src="resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"></script>
<script type="text/javascript" src="resources/bower_components/dtGrid/i18n/zh-cn.js"></script>
<script type="text/javascript" src="resources/bower_components/datePicker/WdatePicker.js" defer="defer"></script>

<script type="text/javascript">
  function checkOperateTicket(){
    with(document.forms[0]){
      if(operateTicketName.value == ""){
        alert('操作票名称必须填写！');
        return false;
      }
    }
    document.getElementById('submit').click();
    return true;
  }

</script>


<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>


  &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-22">
        <h2 class="page-header">入库信息编辑</h2>
      </div>
      <!-- /.col-lg-22 -->
    </div>
    <form:form cssClass="form-horizontal" action="materialStoreInEdit" commandName="materialStoreInEditForm">
      <form:hidden path="id"/>

      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">入库时间</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input cssClass="form-control laydate-icon layer-date" placeholder="入库时间" path="inTime" />
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">入库数量</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input type="text" cssClass="form-control" path="inNum"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">价格</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input type="text" cssClass="form-control" path="inPrice"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">备注</label>
        </div>
        <div class="col-xs-8 col-lg-5">
          <form:textarea  cssClass="form-control" placeholder="备注" path="comment" />
        </div>
      </div>

      <form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkOperateTicket();"/>
      <input class="btn" type="button" value="返回" onclick="history.go(-1);" />
    </form:form>
    <script type="text/javascript">

      $(function() {

        var in_time = {
          elem: '#inTime',
          format: 'YYYY-MM-DD hh:mm:ss',
          max: '2099-06-16',
          istime: true,
          istoday: true,
        };

        laydate(in_time);
      });
    </script>

  <%--</div>

</div>--%>

</body>

</html>


