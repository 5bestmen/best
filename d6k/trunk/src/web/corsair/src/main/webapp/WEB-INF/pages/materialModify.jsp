<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/10/27
  Time: 14:04
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

  <title>物资</title>
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
    var mList =[];
    var eList = [];

    function setModelLable(obj){
      if(obj != null){
        if(obj.className == "tag-txt"){
          obj.className = "tag-txt tag-txt-selected";
          mList.push(obj.innerHTML);
        }
        else{
          obj.className = "tag-txt";
          mList.remove(obj.innerHTML);
        }
      }
    }

    function setExtendLable(obj){
      if(obj!= null){
        if(obj.className == "tag-txt"){
          obj.className = "tag-txt tag-txt-selected";
          eList.push(obj.innerHTML);
        }
        else{
          obj.className = "tag-txt";
          eList.remove(obj.innerHTML);
        }
      }
    }



    function checkMaterialModify(){
      with(document.forms[0]){
        var m_type = "<c:out value="${material_Type}"/>";
        if(m_type == "materialModel"){
          if(!codeId.value){
            alert('物料编码必须填写！');
            return false;
          }
          if(materialTypeId.value == -1){
            alert('请选择物料类别！');
            return false;
          }

          if((mList.length == 0) && (document.getElementById("model_textArea").value =="")){
            alert('物料描述不为空！');
            return false;
          }
          materialModelLables.value = mList.join(',');


          if(document.getElementById('materialModelnameError').innerText){
            alert(document.getElementById('materialModelnameError').innerText);
            return false;
          }
        }
        else{
          if(!fixedId.value){
            alert('固定ID必须填写！');
            return false;
          }
          if(materialModelId.value == -1){
            alert('请选择物料描述！');
            return false;
          }

          materialExtendLables.value = eList.join(',');

          if(document.getElementById('materialnameError').innerText){
            alert(document.getElementById('materialnameError').innerText);
            return false;
          }
        }
     }
      document.getElementById('submit').click();
      return true;
    }

    function checkMaterialModelName(){
      var codeID = document.forms[0].codeId.value.trim();
      if(codeID){
//        eval("var params = {materialName:'"+encodeURIComponent(codeID)+"', mType:'materialModel'}");
//        Ext.Ajax.request({
//          method : 'POST',
//          url: '/api/checkMaterialName',
//          success:checkMaterialNameAndBack,
//          failure:function(){},
//          params:params
//        });
        $.ajax({
          type: 'POST',
          url: '/api/material/checkMaterialName',
          data: {materialName:codeID, mType:'materialModel'},
          success: function (response) {

            if(response == '1'){
              document.getElementById('materialModelnameError').innerText='该物料ID已存在！';
            }
            else if(response == '0'){
              document.getElementById('materialModelnameError').innerText='';
            }
          }
        });
      }
    }

    function checkMaterialName(){
      var fixedID = document.forms[0].fixedId.value.trim();
      if(fixedID){
//        eval("var params = {materialName:'"+encodeURIComponent(fixedID)+"', mType:'material'}");
//        Ext.Ajax.request({
//          method : 'POST',
//          url: '/api/checkMaterialName',
//          success:checkMaterialAndBack,
//          failure:function(){},
//          params:params
//        });
        $.ajax({
          type: 'POST',
          url: '/api/material/checkMaterialName',
          data: {materialName:fixedID, mType:'materialModel'},
          success: function (response) {

            if(response == '1'){
              document.getElementById('materialnameError').innerText='该物料ID已存在！';
            }
            else if(response == '0'){
              document.getElementById('materialnameError').innerText='';
            }
          }
        });
      }
    }

//    function checkMaterialNameAndBack(request){
//      if(request.responseText){
//        var isExist = request.responseText;
//        if(isExist=='1'){
//          document.getElementById('materialModelnameError').innerText='该物料ID已存在！';
//        }
//        else if(isExist=='0'){
//          document.getElementById('materialModelnameError').innerText='';
//        }
//      }
//    }
//    function checkMaterialAndBack(request){
//      if(request.responseText){
//        var isExist = request.responseText;
//        if(isExist=='1'){
//          document.getElementById('materialnameError').innerText='该物料ID已存在！';
//        }
//        else if(isExist=='0'){
//          document.getElementById('materialnameError').innerText='';
//        }
//      }
//    }

    function onCheckLabel(){
       var material_Type = "<c:out value="${material_Type}"></c:out>"
       if(material_Type == "materialModel"){
          var modelSels = "<c:out value="${modelLabelSelList}"></c:out>"
          var modeSelsList = modelSels.split(",");
          if(modeSelsList.length == 0) return;
          var a_List = $("a.tag-txt");
          for(var j = 0; j <a_List.length; j++){
            if($.inArray(a_List[j].innerHTML, modeSelsList) >= 0) {
              a_List[j].className = "tag-txt tag-txt-selected"
              mList.push(a_List[j].innerHTML);
            }
          }
        }
        else{
         var extendSels = "<c:out value="${extendLabelSelList}"></c:out>"
         var extendSelsList = extendSels.split(",");
         if(extendSelsList.length == 0) return;
         var e_List = $("a.tag-txt");
         for(var k = 0; k <e_List.length; k++){
           if($.inArray(e_List[k].innerHTML, extendSelsList) >= 0) {
             e_List[k].className = "tag-txt tag-txt-selected"
             eList.push(e_List[k].innerHTML);
           }
         }
        }
      }

    Array.prototype.remove = function(val) {
      var index = this.indexOf(val);
      if (index > -1) {
        this.splice(index, 1);
      }
    };

//    Array.prototype.indexOf = function(val) {
//      for (var i = 0; i < this.length; i++) {
//        if (this[i] == val) return i;
//      }
//      return -1;
//    };

  </script>

</head>
<body onload="onCheckLabel()">
  <%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>
      <div id="page-wrapper">--%>
        <div class="row">
          <div class="col-lg-12">
            <h2 class="page-header">物资维护<span id="titleType" style="display: none">${material_Type}</span></h2>
          </div>
        </div>
        <form:form cssClass="form-horizontal" action="materialModify" commandName="materialModifyForm">
          <form:hidden path="id"/>
          <form:hidden property="editId" path="editId"/>
          <form:hidden property="mType" path="mType"/>
          <form:hidden property="nodeId" path="nodeId"/>
          <c:if test="${empty material_Type}"></c:if>
          <c:if test="${material_Type eq 'materialModel'}">
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">物料编码</label>
                <span style="color:red" title="必填">(*)</span>
              </div>
              <div class="col-xs-4">
                <form:input path="codeId" cssClass="form-control" onblur="checkMaterialModelName()"/>
              </div>
              <span id="materialModelnameError" style="color:red"></span>
            </div>
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">物料描述</label>
                <span style="color:red" title="必填">(*)</span>
              </div>
              <div id="modelLables" class="col-xs-4">
                  <div class="summary-cont">
                    <div class="sumy-tags">
                        <div class="tag-item">
                          <c:if test="${not empty modelLableList}">
                            <c:forEach items="${modelLableList}" var="modelLable">
                              <a id="m${modelLable.materialModelLableID}" class="tag-txt" href="javascript:void(0);" onclick="setModelLable(this);return false"><c:out value="${modelLable.name}"/></a>
                            </c:forEach>
                          </c:if>
                        </div>
                    </div>
                    <div class="sumy-area">
                       <form:textarea id="model_textArea" path="addModelLables" cssClass="text-area" lang="" cols="30" rows="10" placeholder="没有合适的物料描述？请输入新的物料描述吧~"></form:textarea>
                    </div>
                  </div>
                <form:hidden property="materialModelLables" path="materialModelLables"/>
              </div>
            </div>
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">物料类别</label>
              </div>
              <div class="col-xs-4">
                <form:select path="materialTypeId" cssClass="form-control">
                  <form:option value="-1" label="--请选择类别"/>
                  <form:options items="${typeList}" itemValue="materialTypeID" itemLabel="comments"/>
                </form:select>
              </div>
            </div>
          </c:if>
          <c:if test="${material_Type eq 'material'}">
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">固定ID</label>
                <span style="color:red" title="必填">(*)</span>
              </div>
              <div class="col-xs-4">
                <form:input cssClass="form-control" path="fixedId" onblur="checkMaterialName()"/>
              </div>
              <span id="materialnameError" style="color:red"></span>
            </div>
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">扩展描述</label>
                <%--<span style="color:red" title="必填">(*)</span>--%>
              </div>
              <div id="extendLables" class="col-xs-4">
                <div class="summary-cont">
                  <div class="sumy-tags">
                    <div class="tag-item">
                      <c:if test="${not empty extendLableList}">
                        <c:forEach items="${extendLableList}" var="extendLable">
                          <a id="e${extendLable.materialExtendLableID}" class="tag-txt" href="javascript:void(0);" onclick="setExtendLable(this);return false"><c:out value="${extendLable.name}"/></a>
                        </c:forEach>
                      </c:if>
                    </div>
                  </div>
                  <div class="sumy-area">
                    <form:textarea path="addExtendLables" cssClass="text-area" lang="" cols="30" rows="10" placeholder="没有合适的物料扩展描述？请输入新的扩展描述吧~"></form:textarea>
                  </div>
                </div>
                <form:hidden property="materialExtendLables" path="materialExtendLables"/>
              </div>
            </div>
            <div class="form-group">
              <div class="col-sm-1">
                <label class="control-label">物料描述</label>
              </div>
              <div class="col-xs-4">
                <form:select path="materialModelId" cssClass="form-control">
                  <form:option value="-1" label="--请选择描述"/>
                  <form:options items="${modelList}" itemValue="id" itemLabel="desc"/>
                </form:select>
              </div>
            </div>
          </c:if>
          <div class="col-sm-1"></div>
          <input type="button" class="btn" value="返回" onclick="history.go(-1);"/>
          <form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkMaterialModify();" />
        </form:form>
     <%-- </div>
  </div>--%>
</body>
</html>
