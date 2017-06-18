<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/10/26
  Time: 11:12
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

    <title>物资类别维护</title>
    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

    <script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>
    <script src="<c:url value="/resources/js/common.js"/>"></script>
    <script>
      function checkmaterialTypeModify(){
          with(document.forms[0]){
              if(!materialTypeName.value){
                  alert('类别ID必须填写！');
                  return false;
              }
              if(!materialTypeDesc.value){
                  alert('类别描述必须填写！');
                  return false;
              }
              //检查用户登录名和用户显示名是否重复
              if(document.getElementById('materialTypeNameError').innerText){
                  alert(document.getElementById('materialTypeNameError').innerText);
                  return false;
              }
              document.getElementById('submit').click();
              return true;
         }
      }
     function checkmaterialTypeName(){
         var materialTypeName = document.forms[0].materialTypeName.value.trim();
         if(materialTypeName){
//             eval("var params = {materialName:'"+encodeURIComponent(materialTypeName)+"', mType:'materialType'}");
//             Ext.Ajax.request({
//                 method : 'POST',
//                 url: '/api/checkMaterialName',
//                 success:checkMaterialTypeNameAndBack,
//                 failure:function(){},
//                 params:params
//             });

             $.ajax({
                 type: 'POST',
                 url: '/api/material/checkMaterialName',
                 data: {materialName:materialTypeName, mType:'materialType'},
                 success: function (response) {

                     if(response=='1'){
                         document.getElementById('materialTypeNameError').innerText='该物料类别已存在！';
                     }
                     else if(response=='0'){
                         document.getElementById('materialTypeNameError').innerText='';
                     }
                 }
             });
         }
     }
//      function checkMaterialTypeNameAndBack(request){
//          if(request.responseText){
//              var isExist = request.responseText;
//              if(isExist=='1'){
//                  document.getElementById('materialTypeNameError').innerText='该物料类别已存在！';
//              }
//              else if(isExist=='0'){
//                  document.getElementById('materialTypeNameError').innerText='';
//              }
//          }
//      }
    </script>
</head>
<body>
<%--  <div id="wrapper">
    <jsp:include page="meun.jsp"/>
      <div id="page-wrapper">--%>
        <div class="row">
          <div class="col-lg-12">
            <h2 class="page-header">物资类别维护</h2>
          </div>
        </div>
        <form:form cssClass="form-horizontal" action="materialTypeModify" method="post" commandName="materialTypeModifyForm">
          <form:hidden property="id" path="id"/>
          <div class="form-group">
            <div class="col-sm-1">
              <label class="control-label">类别ID</label>
              <span style="color:red" title="必填">(*)</span>
            </div>
            <div class="col-xs-3">
              <form:input cssClass="form-control" property="materialTypeName" path="materialTypeName" onblur="checkmaterialTypeName()"/>
            </div>
              <span id="materialTypeNameError" style="color:red"></span>
          </div>
            <div class="form-group">
            <div class="col-sm-1">
              <label class="control-label">类别描述</label>
              <span style="color:red" title="必填">(*)</span>
            </div>
            <div class="col-xs-3">
              <form:input cssClass="form-control" property="materialTypeDesc" path="materialTypeDesc"/>
            </div>
          </div>
          <div class="col-sm-1"></div>
          <input type="button" class="btn" value="返回" onclick="history.go(-1);" />
          <form:input type="submit" cssClass="btn"  path="submit" value="保存"  onclick="return checkmaterialTypeModify();"/>

        </form:form>
     <%-- </div>
  </div>--%>
</body>
</html>
