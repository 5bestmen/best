<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair" %>
<%@ page import="org.springframework.web.context.support.WebApplicationContextUtils" %>
<%@ page import="com.corsair.service.ApplicationInformationService" %>
<%
    ApplicationInformationService service = (ApplicationInformationService) WebApplicationContextUtils
            .getRequiredWebApplicationContext(session.getServletContext()).getBean("applicationInformationService");

    String appName = service.getAppName();
%>
<%
    session.getAttributeNames();
%>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>${sessionScope.APP_NAME}-综合数据</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/commonStyle.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/img/sun.png"/>" rel="shortcut icon"/>

    <script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>

    <link href="<c:url value="/resources/css/commonStyle.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/tree/easyui.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/tree/icon.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/tree/demo.css"/>" rel="stylesheet" type="text/css">

    <%--todo 页面自身需要的CSS begin--%>
    <%--页面自身需要的CSS end--%>
</head>
<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/tree/jquery.easyui.min.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script src="<c:url value="/resources/dist/js/echarts/echarts.js"/>"></script>


<%--<script type="application/javascript">
  $(function () {
    var Chat = {};

    Chat.socket = null;

    Chat.connect = (function (host) {
      if ('WebSocket' in window) {
        Chat.socket = new WebSocket(host);
      } else if ('MozWebSocket' in window) {
        Chat.socket = new MozWebSocket(host);
      } else {
        //Console.log('Error: WebSocket is not supported by this browser.');
        return;
      }

      Chat.socket.onopen = function () {
        //Console.log('Info: WebSocket connection opened.');

      };

      Chat.socket.onclose = function () {
        //Console.log('Info: WebSocket closed.');
        Chat.initialize();
      };

      Chat.socket.onmessage = function (message) {
        Console.log(message.data);
      };
    });

    Chat.initialize = function () {
      if (window.location.protocol == 'http:') {
        Chat.connect('ws://10.1.9.28:8081/event?type=0');
      } else {
        Chat.connect('wss://' + window.location.host + '/event');
      }
    };

    var Console = {};

    Console.log = (function (message) {


      var tbody = document.getElementById('eventbody');

      var evt = JSON.parse(message);

      var tr = document.createElement("tr");

      var td1 = document.createElement("td");
      td1.style.width = "40%";


      if (tbody != null) {
        for (var i = 0; i < tbody.rows.length; i++) {
          if (tbody.rows[i].rowIndex % 2 == 1) {
            tbody.rows[i].style.backgroundColor = "";
          }
          else {
            tbody.rows[i].style.backgroundColor = "#82accb";
          }
        }
      }


      var ymdstr = evt.ymd.toString();
      var hmsstr = evt.hmsms.toString();
      if (hmsstr.length < 9) {
        hmsstr = "0" + hmsstr;
      }

      td1.innerHTML = ymdstr.substr(0, 4) + "-" + ymdstr.substr(4, 2) + "-" + ymdstr.substr(6, 2)
      + " " + hmsstr.substr(0, 2) + ":" + hmsstr.substr(2, 2) + ":" + hmsstr.substr(4, 2);

      tr.appendChild(td1);
      var td2 = document.createElement("td");
      td2.style.width = "60%";
      td2.innerHTML = evt.text;
      tr.appendChild(td2);
      /*var td3 = document.createElement("td");
       td3.innerHTML = evt.Object;
       tr.appendChild(td3);*/

      tbody.appendChild(tr);

      while (tbody.childNodes.length > 100) {
        tbody.removeChild(tbody.firstChild.nextSibling);
      }

      Chat.socket.send(message);

    });

    Chat.initialize();

  });
</script>--%>
<script>
    $(function () {
        //默认打开是主页面
        $("iframe[name='main']").attr("src", "index");
        var a = "综合数据";
        $("#getTitle").append("<li><a href='index'> " + a + "</a></li>");
        //滚动条的隐藏
//    document.body.style.overflow='hidden';

        //面包屑
        $(".title").click(function () {
            var i = 0;
            var con = $(this).html();
            var con1 = $(this).children(0).attr('href');
            //#getTitle代表页面中想要得到title的值元素
            $("#getTitle").html(con);
            $("#getTitle").children(0).attr("href", con1);
        });

        $(".special").click(function () {
            var i = 0;
            var con = $(this).html();
            var con1 = $(this).children(0).attr('href');
            if ($(this).parent().parent().find("ul").length > 0) {
                var ele = $(this).parent().parent().children(0).html();
                var a = ele.substring(0, ele.indexOf("<"));
                i++;
            }
            //#getTitle代表页面中想要得到title的值元素
            $("#getTitle").html("");
            $("#getTitle").append("<li>" + a + "</li><li>" + con + "</li>");
            $("#getTitle").children(1).attr("href", con1);
        })

    });
</script>
<script type="text/javascript" language="javascript">
    var browserVersion = window.navigator.userAgent.toUpperCase();
    var isOpera = false;
    var isFireFox = false;
    var isChrome = false;
    var isSafari = false;
    var isIE = false;
    var iframeTime;
    function reinitIframe(iframeId, minHeight) {
        try {
            var iframe = document.getElementById(iframeId);
            var bHeight = 0;
            if (isChrome == false && isSafari == false)
                bHeight = iframe.contentWindow.document.body.scrollHeight;

            var dHeight = 0;
            if (isFireFox == true)
                dHeight = iframe.contentWindow.document.documentElement.offsetHeight + 2;
            else if (isIE == false && isOpera == false)
                dHeight = iframe.contentWindow.document.documentElement.scrollHeight;
            else
                bHeight += 3;
            var height = Math.max(bHeight, dHeight);
            if (height < minHeight) height = minHeight;
            iframe.style.height = height + "px";
        } catch (ex) {
        }
    }
    function startInit(iframeId, minHeight) {
        isOpera = browserVersion.indexOf("OPERA") > -1 ? true : false;
        isFireFox = browserVersion.indexOf("FIREFOX") > -1 ? true : false;
        isChrome = browserVersion.indexOf("CHROME") > -1 ? true : false;
        isSafari = browserVersion.indexOf("SAFARI") > -1 ? true : false;
        if (!!window.ActiveXObject || "ActiveXObject" in window)
            isIE = true;
        reinitIframe(iframeId, minHeight);
        if (iframeTime != null)
            clearInterval(iframeTime)
        iframeTime = window.setInterval("reinitIframe('" + iframeId + "'," + minHeight + ")", 100);
    }
    startInit('mainFrame', 560);
</script>
<script>
    var treeData;
    $.ajax({
        type: "GET",
        url: "/api/psr/getTreeData",
        contentType: "application/json",
        dataType: "json",
        async: false,
        success: function (data) {
            treeData = data;
        }
    });
    function getFirst(node, data) {
        if (data.length > 0) {
            var n = $('.easyui-tree').tree('find', data[0].id);
            $('.easyui-tree').tree('select', n.target);
        }
    }

    function refresh() {
        document.getElementById('mainFrame').contentWindow.refresh();
    }

    function getTreeText(){
        return $('.easyui-tree').tree('getSelected').text;
    }

    function getTreeLevel(){
        return $('.easyui-tree').tree('getSelected').level;
    }

    function getTreeStationID(){
        return $('.easyui-tree').tree('getSelected').stationID;
    }

    function getTreeStationName(){
        return $('.easyui-tree').tree('getSelected').stationName;
    }

    function getTreeDeviceID(){
        return $('.easyui-tree').tree('getSelected').deviceID;
    }

    function getDeviceInfo() {
        var level = getTreeLevel();
        if (level != 4) {
            return;
        }

        var stationID = getTreeStationID();
        var deviceID = getTreeDeviceID();

        $.ajax({
            type : "POST",
            url: "/api/multiCharts/getDeviceInfo",
            async : false,
            data : {
                "stationID": stationID,
                "deviceID": deviceID,
            },
            success : function(result) {
                if (result == null || result.length == 0) {
                    return;
                }

                if (result[0] == null || result[0].length <= 0) {
                    alert("没有配置设备信息!");
                    return;
                }

                alert(result[0]);
            },
            error: function (errorMsg) {
                swal({
                    title: "不好意思，请求数据失败啦!",
                    text: "不好意思，请求数据失败啦!",
                    confirmButtonText: "确定"
                });
            }
        });
    }
</script>


<div id="wrapper">
    <div class="row" style="background: url(/resources/img/top_bg.png) no-repeat;background-size:cover ;">
        <div class="panel-heading" style="padding: 10px 25px;">
            <%--<div style="float: left"><span class="panel-title" style="font-size: 36px;padding: 0px 600px"><%=appName%></span>--%>
            <div class="panel-title" align="center" style="font-size: 36px;color: white"><%=appName%></div>
            <div style="float: right">
                <ul class="nav navbar-top-links navbar-right">

                    <%--上部导航条的用户部分--%>
                    <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                    <i class="fa fa-user fa-fw"></i>${com_user.account.realname}<i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                    <%--<li><a href="#"><i class="fa fa-user fa-fw"></i>${com_user.account.realname}</a>--%>
                    <li><a href="login?flag=0"><i class="fa fa-sign-out fa-fw"></i>注销</a>
                    <%--<li><a href="pwdModify" target="main"><i class="fa fa-wrench fa-fw"></i>修改口令</a>a--%>
                    </li>
                    </ul>
                    <!-- /.dropdown-user -->
                    </li>
                    <!-- /.dropdown -->
                </ul>
            </div>
        </div>
    </div>
    <div style="float:left;width: 20%">
        <ul class="easyui-tree" data-options="
            data:treeData,
            method:'get',
            animate:true,
            formatter:function(node){
                var s = node.text;
                if (node.children){
                    s += ' <span style=\'color:blue\'>(' + node.children.length + ')</span>';
                }
                return s;
            },
            onClick:function(node){
                refresh();
            },
            onDblClick:function(node){
                getDeviceInfo();
            },
            onLoadSuccess: function (node, data) {
                getFirst(node, data);
            }
        ">
        </ul>
    </div>
    <div style="float:left;width: 80%;white-space: nowrap;">
        <jsp:include page="meun.jsp"/>
        <div id="page-wrapper">
            <div style="border-bottom: 1px solid;">
                <ol class="breadcrumb" id="getTitle">
                </ol>
            </div>

            <iframe name="main" id="mainFrame" width="100%" height="1000px" frameborder="0" align="right" scrolling="no">

            </iframe>

        </div>
    </div>


</div>
</body>
</html>
