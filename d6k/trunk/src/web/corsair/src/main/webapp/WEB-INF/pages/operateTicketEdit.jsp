<%--
  Created by IntelliJ IDEA.
  User: zjq
  Date: 2015/10/22
  Time: 14:56
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

  <title>生成操作票</title>

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
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

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


<%--<div id="wrapper">--%>
  <%--<jsp:include page="meun.jsp"/>--%>


  <%--&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;--%>
  <%--<div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-22">
        <h3 class="page-header">操作票编辑</h3>
      </div>
      <!-- /.col-lg-22 -->
    </div>
    <form:form cssClass="form-horizontal" action="operateTicketEdit" commandName="operateTicketEditForm">
      <form:hidden path="id"/>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">操作票名称</label>
          <span style="color:red" title="必填">(*)</span>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input type="text" cssClass="form-control" path="operateTicketName"/>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">发令人</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control"  path="sendName" items="${accoutRealNames}"></form:select>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">受令人</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control"  path="recvName" items="${accoutRealNames}"></form:select>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">发令时间</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input cssClass="form-control laydate-icon layer-date" placeholder="发令时间" path="sendTime" />
        </div>
      </div>
     <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">操作开始时间</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input cssClass="form-control laydate-icon layer-date" placeholder="开始操作时间" path="beginTime" />
        </div>
      </div>
     <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">操作结束时间</label>
        </div>
        <div class="col-xs-8 col-lg-3">
          <form:input cssClass="form-control laydate-icon layer-date" placeholder="结束操作时间" path="endTime" />
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">监护人</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control"  path="guardianName" items="${accoutRealNames}"></form:select>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">操作员</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control"  path="singleOperator" items="${accoutRealNames}"></form:select>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">检修员</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control"  path="repairOperator" items="${accoutRealNames}"></form:select>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-2 col-lg-2">
          <label class="control-label">操作任务</label>
        </div>
        <div class="col-xs-8 col-lg-5">
          <form:textarea  cssClass="form-control" placeholder="任务内容" path="operateTask" />
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

      <div id = "ticket">
        <c:if test="${not empty operateTicketTemplateDatas}">
          <c:forEach items="${operateTicketTemplateDatas}" var="operateTicketTemplateData">

            <div class="form-group items">
              <div class="col-sm-2 col-lg-2">
                <label class="control-label">操作项目</label>
              </div>
              <div class="col-lg-5 col-xs-8">
                <textarea  class="form-control"name="operateColumn" rows="2" cols="300" type="text" placeholder="操作项目">${operateTicketTemplateData.content}</textarea>
              </div>
              <div class="col-lg-1 col-xs-1">
                <button type="button"  class="del-item btn btn-danger">删除</button>
              </div>
              <div class="col-lg-1 col-xs-1">
                <button type="button"  class="up-item btn btn-warning">上移</button>
              </div>
              <div class="col-lg-1 col-xs-1">
                <button type="button"  class="down-item btn btn-warning">下移</button>
              </div>
            </div>

          </c:forEach>
        </c:if>
      </div>
        <div class="form-group">
            <div class="col-xs-6">
                <button type="button" id="addItem" class="btn btn-primary">增加操作项目</button>
            </div>
        </div>
        <c:if test="${not empty operateTicketEditForm.isCheckAuthority}">
            <form:input type="submit" cssClass="btn btn-success"  path="checkSuccess" value="审核通过" />
            <form:input type="submit" cssClass="btn btn-failed"  path="checkFailed" value="审核不通过" />
        </c:if>
        <c:if test="${empty operateTicketEditForm.isCheckAuthority}">
            <form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkOperateTicket();"/>
        </c:if>

      <input class="btn" type="button" value="返回" onclick="history.go(-1);" />
    </form:form>
    <script>

      $(function() {
        var max_item_num = 15;
        var item_num = $('#ticket .items').length;
        if (item_num > 0){
          $('.del-item').click(function (){
            $(this).parent().parent().remove();
            item_num--;
          });

          if (item_num > 1){
            $('.up-item').click(function (){
              var current_div =  $(this).parent().parent();
              if (current_div.index() > 0){
                current_div.prev().before(current_div);
              }
            });
            $('.down-item').click(function (){
              var current_div =  $(this).parent().parent();
              if (current_div.index() < item_num-1){
                current_div.next().after(current_div);
              }
            });
          }
        }

        $('#addItem').click(function () {

          if (item_num < max_item_num) {
            $('#ticket').append('<div class="form-group items">'+
                    '<div class="col-sm-2 col-lg-2">'+
                    '<label class="control-label">操作项目</label>'+
                    '</div>'+
                    '<div class="col-lg-5 col-xs-8">'+
                    '<textarea  class="form-control" name="operateColumn" rows="2" cols="300" type="text" placeholder="操作项目"></textarea>'+
                    '</div>'+
                    '<div class="col-lg-1">'+
                    '<button type="button"  class="del-item btn btn-danger">删除</button>'+
                    '</div>'+
                    '<div class="col-lg-1">'+
                    '<button type="button"  class="up-item btn btn-warning">上移</button>'+
                    '</div>'+
                    '<div class="col-lg-1">'+
                    '<button type="button"  class="down-item btn btn-warning">下移</button>'+
                    '</div>'+
                    '</div>');
            item_num++;

          } else {
            alert("最多加15个");
          }
          $('.del-item').unbind();
          $('.del-item').on('click', function () {
            $(this).parent().parent().remove();
            item_num--;
          });
          $('.up-item').unbind();
          $('.up-item').on('click', function () {
            var current_div =  $(this).parent().parent();
            if (current_div.index() > 0){
              current_div.prev().before(current_div);
            }
          });
          $('.down-item').unbind();
          $('.down-item').on('click', function () {
            var current_div =  $(this).parent().parent();
            if (current_div.index() < item_num-1){
              current_div.next().after(current_div);
            }
          });
        });

      });

      var send_time_data_diy = {
          elem: '#sendTime',
          format: 'YYYY-MM-DD hh:mm:ss',
          max: '2099-06-16',
          istime: true,
          istoday: true,
      }
      var begin_time_data_diy = {
          elem: '#beginTime',
          format: 'YYYY-MM-DD hh:mm:ss',
          max: '2099-06-16',
          istime: true,
          istoday: true,

      }
      var end_time_data_diy = {
          elem: '#endTime',
          format: 'YYYY-MM-DD hh:mm:ss',
          max: '2099-06-16',
          istime: true,
          istoday: true,

      }
      laydate(send_time_data_diy);
      laydate(begin_time_data_diy);
      laydate(end_time_data_diy);
    </script>

  <%--</div>

</div>
--%>
</body>

</html>


