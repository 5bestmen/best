<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/10/20
  Time: 10:05
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%
    session.getAttributeNames();
%>
<html>
<head>
    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/extjs/resources/css/ext-all.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <title>物资分类</title>

    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/bootstrap-treeview.min.js"/>"></script>

    <script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>
    <script src="<c:url value="/resources/js/common.js"/>"></script>
    <script type="text/javascript">
        var materialData ={};

        function initTable(materialData){
            $.post('/api/material/getMaterialPsrList', {
                dataID: materialData.dataID,
                dateType: materialData.dataType,
                dataDesc: materialData.queryDesc,
                dataExtendDesc:materialData.queryextendDesc,
                queryFlag:materialData.queryFlag
            },function (response) {
                $('#materialTable').remove();
                var _table = document.createElement("table");
                _table.id = "materialTable";
                _table.className = "table table-bordered table-striped";

                <%--表格title--%>
                var _thred = document.createElement("thead");
                var _tr = document.createElement("tr");
                var materialModelTitle = new Array("物料编码","物料描述", "物料类别","编辑","删除");
                var materialTitle = new Array("固定ID","物料描述", "扩展描述","编辑","删除");
                for(var n in materialModelTitle){
                    var _th = document.createElement("th");
                    if(materialData.dataType == "materialType"){
                        _th.innerHTML = materialModelTitle[n];
                        document.getElementById("material_add").setAttribute("href","materialModify?id="+materialData.dataID+"&mType=materialModel");
                    }
                    else if(materialData.dataType == "materialModel"){
                        _th.innerHTML = materialTitle[n];
                        document.getElementById("material_add").setAttribute("href","materialModify?id="+materialData.dataID+"&mType=material");
                    }
                    else{
                        _th.innerHTML = materialTitle[n];
                    }
                    _tr.appendChild(_th);
                }
                _thred.appendChild(_tr);
                _table.appendChild(_thred);

                <%--表格body--%>
                var _tbody = document.createElement("tbody");
                for(var j in response){
                    var _btr = document.createElement("tr");
                    var _btd1 = document.createElement("td");
                    var _btd2 = document.createElement("td");
                    var _btd3 = document.createElement("td");
                    var _btd4 = document.createElement("td");
                    var _btd5 = document.createElement("td");

                    <%--编辑按钮--%>
                    var _aEdit = document.createElement("a");
                    _aEdit.className = "btn btn-primary";
                    _aEdit.text = "编辑 ";

                    <%--删除按钮--%>
                    var _aDel = document.createElement("a");
                    _aDel.className = "btn btn-warning";
                    _aDel.text = "删除 ";

                    if(materialData.dataType == "materialType"){
                        _btd1.innerHTML = response[j].name;
                        _btd2.innerHTML = response[j].desc;
                        _btd3.innerHTML = response[j].type;
                        var aEdit_href = "materialModify?editId=" + response[j].id+"&mType=materialModel&materialModelLables="+response[j].desc+"&nodeId="+materialData.nodeId;
                        _aEdit.href = encodeURI(encodeURI(aEdit_href));
                        _aDel.href = "javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='" + response[j].id +"';document.forms[0].del.value = 'materialModel';document.forms[0].del.click();}";
                    }
                    else if(materialData.dataType == "materialModel"){
                        _btd1.innerHTML = response[j].name;
                        _btd2.innerHTML = response[j].type;
                        _btd3.innerHTML = response[j].desc;
                        var eEdit_href = "materialModify?id="+materialData.dataID+"&editId=" + response[j].id+"&mType=material&materialExtendLables="+response[j].desc+"&nodeId="+materialData.nodeId;
                        _aEdit.href = encodeURI(encodeURI(eEdit_href));
                        _aDel.href = "javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='" + response[j].id +"';document.forms[0].del.value = 'material';document.forms[0].del.click();}";
                    }
                    _btd4.align = "center";
                    _btd5.align = "center";

                    var _iEdit = document.createElement("i");
                    _iEdit.className = "glyphicon glyphicon-edit";
                    _aEdit.appendChild(_iEdit);
                    _btd4.appendChild(_aEdit);

                    var _iDel = document.createElement("i");
                    _iDel.className = "glyphicon glyphicon-remove";
                    _aDel.appendChild(_iDel);
                    _btd5.appendChild(_aDel);

                    _btr.appendChild(_btd1);
                    _btr.appendChild(_btd2);
                    _btr.appendChild(_btd3);
                    _btr.appendChild(_btd4);
                    _btr.appendChild(_btd5);
                    _tbody.appendChild(_btr);
                }
                _table.appendChild(_tbody);
                document.getElementById("materialDiv").appendChild(_table);
            }, 'json');
        }

      function getTree() {

              $('#menuTree').treeview({
                  data: ${menuJson},
                  nodeIcon: 'glyphicon glyphicon-stop',
//          showIcon: true,
                  showCheckbox: false,
                  onNodeSelected: function (event, node){
//              document.forms[0].style.display = 'none';
                      materialData.dataType = node.area;
                      materialData.dataID = node.id;
                      materialData.queryFlag = false;
                      materialData.nodeId = node.nodeId;
                      initTable(materialData);
                  }
              });

            var nodeIdSelect = "<c:out value="${sessionScope.nodeSelected}"></c:out>";
              if(nodeIdSelect.length == 0){
                  nodeIdSelect = "0";
              }
               $('#menuTree').treeview('selectNode', $('#menuTree').treeview('getNode', parseInt(nodeIdSelect)));

//         $('#menuTree').treeview('expandAll');
      }

        function materialTypeEdit(){
            if(materialData.dataID != null){
                if(materialData.dataType == "materialType"){
                    window.location.href = "materialTypeModify?id=" + materialData.dataID;
                }
                else{
                    alert("请选择物资类别!");
                }
            }
        }

        function materialTypeDel(){
            if(materialData.dataID != null){
                if(materialData.dataType == "materialType"){
                    if(confirm('是否确定删除这条记录？')){
                        document.forms[0].deleteIds.value=materialData.dataID;
                        document.forms[0].del.value = "materialType";
                        document.forms[0].del.click();}
                }
                else{
                    alert("请选择物资类别!");
                }
            }
        }

        function onSecrch(){
            materialData.queryFlag = true;
            materialData.queryDesc =  document.forms[0].describe.value;
            materialData.queryextendDesc = document.forms[0].extendDesc.value;
            if(materialData.queryDesc.length != 0 &&  materialData.queryextendDesc.length == 0){
                materialData.dataType = "materialType";
            }
            else if(materialData.queryDesc.length == 0 &&  materialData.queryextendDesc.length != 0){
                materialData.dataType = "materialModel";
            }

            initTable(materialData);
        }

        window.onload=function(){getTree();}
    </script>

</head>
<body>
<%--  <div id="wrapper">
    <jsp:include page="meun.jsp"/>
    <div id="page-wrapper">--%>
      <div class="row">
        <div class="col-lg-12">
          <h2 class="page-header">物资分类</h2>
        </div>
      </div>
      <div class="row">
        <div class="col-lg-12">
          <div class="panel panel-primary">
            <div class="panel-heading">
              <i class="fa fa-child fa-fw"></i>物资
            </div>
            <div class="panel-body">
              <div class="col-lg-5">
                <a class="btn btn-primary" href="materialTypeModify">
                    <i class="glyphicon glyphicon-plus"></i>
                    新增分类
                </a>
                <a class="btn btn-primary" herf="materialTypeModify"  onclick="materialTypeEdit();return false">
                    <i class="glyphicon glyphicon-edit"></i>
                    编辑分类
                </a>
                <a class="btn btn-primary" herf="materialSort"  onclick="materialTypeDel();return false">
                    <i class="glyphicon glyphicon-remove"></i>
                    删除分类
                </a>
                <div style="margin:10px 0;"></div>
                <div id="menuTree" class="treeview"></div>
              </div>
              <div class="col-lg-7">
                  <form:form cssClass="form-inline" action="materialSort" method="post" commandName="materialForm" >
                      <form:hidden path="deleteIds"/>
                      <form:input type="submit" value="批量删除" cssStyle="display: none" path="del"/>
                      <div class="form-group">
                          <input class="form-control" placeholder="物资描述" value
                                 name="describe" autofocus/>
                      </div>
                      <div class="form-group">
                          <input class="form-control" placeholder="扩展描述" name="extendDesc"/>
                      </div>

                      <button id="material_search"  class="btn btn-primary" onclick="onSecrch();return false">
                          <i class="glyphicon glyphicon-search"></i>
                          查询物资
                      </button>
                      <a id="material_add" class="btn btn-primary" href="materialModify">
                        <i class="glyphicon glyphicon-plus"></i>
                      新增物资
                      </a>

                      <div style="margin:10px 0;"></div>
                      <div class="row">
                          <div class="col-lg-12 col-md-12">
                              <div id="materialDiv" class="table-responsive">
                                  <table id="materialTable" class="table table-bordered table-striped">

                                  </table>
                              </div>
                          </div>
                      </div>
                  </form:form>
              </div>
            </div>
          </div>
        </div>
      </div>
  <%--  </div>
  </div>--%>
</body>
</html>
