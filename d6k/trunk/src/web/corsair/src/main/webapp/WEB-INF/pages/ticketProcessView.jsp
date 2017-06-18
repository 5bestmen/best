<%@ page contentType="text/html;charset=UTF-8"%>
<%@ include file="/common/taglibs.jsp"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="en">
<head>
    <title>流程状态</title>
    <%@ include file="/common/meta.jsp"%>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">

    <link href="<c:url value="/resources/css/snaker/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/snaker/snaker.css"/>" rel="stylesheet" type="text/css">
    <link href="resources/css/style.css" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

    <script src="<c:url value="/resources/js/snaker/raphael-min.js"/>"></script>
    <%--<script src="<c:url value="/resources/js/snaker/jquery-ui-1.8.4.custom/js/jquery.min.js"/>"></script>--%>
    <script src="<c:url value="/resources/js/snaker/jquery-ui-1.8.4.custom/js/jquery-ui.min.js"/>"></script>
    <script src="<c:url value="/resources/js/snaker/snaker/snaker.designer.js"/>"></script>
    <script src="<c:url value="/resources/js/snaker/snaker/snaker.model.js"/>"></script>
    <script src="<c:url value="/resources/js/snaker/snaker/snaker.editors.js"/>"></script>
    <script type="application/javascript">
        $(function(){
            $("tbody tr:even").css("background-color","rgb(31, 86, 134)");
            $("tbody tr:even").children().css("color","white");
            $("tbody tr:odd").css("background","#82accb");
        });

    </script>
    <script type="text/javascript">
        function addTaskActor(taskName) {
            var url = '${ctx}/snaker/task/actor/add?orderId=${order.id}&taskName=' + taskName;
            var returnValue = window.showModalDialog(url,window,'dialogWidth:1000px;dialogHeight:600px');
            if(returnValue) {
                $('#currentActorDIV').append(',' + returnValue);
            }
        }
        function display(process, active) {
            /** view*/
            $('#snakerflow').snakerflow($.extend(true,{
                        basePath : "/resources/js/snaker/snaker/",
                        ctxPath : "${ctx}",
                        orderId : "${order.id}",
                        restore : eval("(" + process + ")")
                        ,
                        editable : false
                    },eval("(" + active + ")")
            ));
        }
    </script>
</head>
<body>
<%--<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>--%>
<%--<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>--%>
<%--<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>--%>
<%--<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>--%>
<%--<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>--%>


<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>

        &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
        <div id="page-wrapper">--%>
            <div class="row">
                <div class="col-lg-12">
                    <h3 class="page-header">流程状态</h3>
                </div>
                <!-- /.col-lg-12 -->
            </div>
            <tr>
                <td class="td_table_top" align="left">
                    流程名称：<font color="red">${order.processName }</font>&nbsp;&nbsp;
                    流程编号：<font color="red">${order.orderNo }</font>&nbsp;&nbsp;
                    流程创建时间：<font color="red">${order.createTime }</font>&nbsp;&nbsp;
                    流程状态：<font color="red">${order.orderState  == 0 ? '已结束' : '运行中' }</font>
                </td>
            </tr>
            <div style="margin:10px 0;"></div>
            <div class="row">
                <div class="col-lg-12 col-md-12">
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped">
                            <thead>
                            <tr id="first">
                                <th>
                                    任务名称
                                </th>
                                <th>
                                    任务创建时间
                                </th>
                                <th>
                                    任务完成时间
                                </th>
                                <th>
                                    任务处理人
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <%--<c:if test="${not empty ticketProcessList}">--%>
                            <c:forEach items="${tasks}" var="item">
                                <tr>
                                    <td>
                                        <c:out value="${item.displayName}"/>
                                    </td>
                                    <td>
                                        <c:out value="${item.createTime}"/>
                                    </td>
                                    <td>
                                        <c:out value="${item.finishTime}"/>
                                    </td>
                                    <td>
                                        <c:out value="${item.operator }"/>
                                    </td>
                                </tr>
                            </c:forEach>
                            <%--</c:if>--%>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
            <div>
                <table align="center" border="0" cellpadding="0" cellspacing="0">
                    <tr>
                        <td align="left">
                            <input class="btn" type="button" value="返回" onclick="history.back();" />
                        </td>
                    </tr>
                </table>
            </div>
            <div>
                <table class="properties_all" align="center" border="1" cellpadding="0" cellspacing="0" style="margin-top: 0px">
                    <div id="snakerflow" style="border: 1px solid #d2dde2; height:800px; margin-top:10px; margin-left:10px; width:98%;">
                    </div>
                </table>
            </div>
        <%--</div>
</div>--%>
<script type="text/javascript">
    $.ajax({
        type:'GET',
        url:"${ctx}/snaker/process/json",
        data:"processId=${processId}&orderId=${order.id}",
        async: false,
        globle:false,
        error: function(){
            alert('数据处理错误！');
            return false;
        },
        success: function(data){
            display(data.process, data.active);
        }
    });
</script>
</body>
</html>
