<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair" %>
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

    <title>${sessionScope.APP_NAME}-主页</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/img/sun.png"/>" rel="shortcut icon"/>
</head>

<body>
    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>

<script>
    //功率曲线
    var eOption = {
        legend: {
            textStyle:{
                color:'white'
            },
            data: ['实时发电功率']
        },
        xAxis: [
            {
                type: 'category',
                boundaryGap: false,
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                data: []
            }

        ],
        yAxis: [
            {
                type: 'value',
                axisLabel: {
                    formatter: '{value} kW',
                    textStyle: {
                        color:'white'
                    }
                }
            }
        ],
        series: [
            {
                name: '实时发电功率',
                type: 'line',
                smooth: true,
                symbol: 'none',
                itemStyle: {
                    normal: {
                        color: '#32d2c9'
                    }
                },
                data: []
            }
        ],
        grid: {
            x: '10%',
            y: '10%',
            x2: '5%',
            y2: '10%'
        }
    };

    var optionCurve = {
        ajax: true,
        URL: '/api/charts/stations/stationPowerCurve',
        robin: true,
        timeSpan: 5000,
        getConfig: function () {
            return {type: 'PowerCurve'};
        },
        formatNumber: true,
        EchartsOption: eOption,
        chartContainer: 'powerCurve'
    };

    $(function () {
        var curveChart = $.fn.GeneralChart.init(optionCurve);
        curveChart.load();
    });
</script>
<script>
    var dayEcOption = {
        tooltip: {
            trigger: 'axis'
        },
        legend: {
            padding: 5,
            textStyle:{
                color:'white'
            },
            data: ['日发电量', "转换效率"]
        },
        toolbox: {
            show: true,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
            /*orient:'vertical',
             x:'right',
             y:'top'*/
        },
        xAxis: [
            {
                type: 'category',
                boundaryGap: true,
                axisLine: {onZero: false},
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                name: '日发电量(kWh)',
                nameTextStyle:{
                    color:'white'
                },
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                type: 'value'
            },
            {
                name: '转换效率(%)',
                nameTextStyle:{
                    color:'white'
                },
                splitLine: {show: false},
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                type: 'value',
                min: 90,
                max: 100
            }
        ],
        series: [
            {
                name: '日发电量',
                type: 'bar',
                itemStyle: {
                    normal: {
                        color: '#32d2c9'
                    }
                },
                data: []
            },
            {
                name: '转换效率',
                type: 'line',
                z: 10,
                smooth: true,
                itemStyle: {
                    normal: {
                        lineStyle: {
                            width: 4
                        },
                        color: '#e2b428'
                    }
                },
                yAxisIndex: 1,
                data: []
            }
        ],
        grid: {
            y: '12%',
            x: '3%',
            x2: '3%',
            y2: '10%'
        }
    };

    var dayOption = {
        ajax: true,
        URL: '/api/charts/stations/inverterDayGenerated',
        robin: true,
        timeSpan: 10000,
        getConfig: function () {
            return {type: 'DayGenerated'};
        },
        formatNumber: true,
        EchartsOption: dayEcOption,
        chartContainer: 'dayPower'
    };

    $(function () {

        $("#power-tab-1").addClass("active");

        var dayChart = $.fn.GeneralChart.init(dayOption);
        dayChart.load();
    });

</script>
<script>
    $(function () {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();
    });
</script>

<script type="application/javascript">
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
                Chat.connect('ws://'+ window.location.host +'/ws/event?type=0');
            } else {
                Chat.connect('wss://'+ window.location.host +'/ws/event?type=0');
            }
        };

        var Console = {};

        Console.log = (function (message) {


            var tbody = document.getElementById('eventbody');

            var evt = JSON.parse(message);

            var tr = document.createElement("tr");

            var td1 = document.createElement("td");
            td1.style.width = "30%";

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

            var td3 = document.createElement("td");
            td3.style.width = "10%";
            var a = document.createElement("a");
            a.setAttribute("class", "btn btn-primary btn-xs");
            a.onclick = function (para){ return function() {a.innerHTML="已确认";
                a.setAttribute("class", "btn btn-primary btn-xs disabled");
                Chat.socket.send(para);} }(message)
            a.innerHTML = "确认";

            td3.appendChild(a);
            tr.appendChild(td3);
            /*var td3 = document.createElement("td");
             td3.innerHTML = evt.Object;
             tr.appendChild(td3);*/

            tbody.appendChild(tr);

            if (tbody != null) {
                for (var i = 0; i < tbody.rows.length; i++) {
                    if (tbody.rows[i].rowIndex % 2 == 1) {
                        tbody.rows[i].style.backgroundColor = "";
                        tbody.rows[i].childNodes[0].style.color = "white";
                        tbody.rows[i].childNodes[1].style.color = "white";
                    }
                    else {
                        tbody.rows[i].style.backgroundColor = "#82accb";

                    }
                }
            }

            while (tbody.childNodes.length > 100) {
                tbody.removeChild(tbody.firstChild.nextSibling);
            }

        });

        Chat.initialize();
    });

</script>

<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>


    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row" style=" margin-left: -15px;">
            <div class="col-lg-12">
                <h3>主面板
                    <%--<span class="alignright">${sessionScope.APP_NAME}</span>--%>
                </h3>
            </div>
        </div>
        <div class="row" style=" margin-left: -5px;">
            <div class="row" style=" margin-left: -5px;">
                <div class="col-md-4 measurement-container">
                    <corsair:measurement displayAreaName="index-station-day"
                                         PSRID="${station.id}"/>
                </div>
                <div class="col-md-4 measurement-container">
                    <corsair:measurement displayAreaName="index-weather"
                                         PSRID="${weatherInstrument.id}"/>
                </div>
                <div class="col-md-4 measurement-container">
                    <corsair:measurement displayAreaName="index-green-info"
                                         PSRID="${packageTransformer.id}"/>
                </div>
            </div>
            <div class="col-md-6 measurement-container" style="width: 50%">
                <div class="panel panel-transparent">
                    <div class="panel-heading">
                        功率曲线
                    </div>
                    <div class="panel-body">
                        <div id="powerCurve" style="height: 350px; width: 100%;"></div>
                    </div>
                </div>
            </div>

            <div class="col-md-6 measurement-container">
                <div class="panel panel-transparent">
                    <div class="panel-heading">事项告警</div>
                    <div class="panel-body">
                        <table id="warning" style="width: 100%; ">
                            <tr id="first">
                                <th>时间</th>
                                <th>内容</th>
                                <%--<th>设备名</th>--%>
                            </tr>
                        </table>
                        <div style="overflow:auto; height: 300px">
                            <div id="dvTbody">
                                <table id="tbTbody" style="width: 100%">
                                    <tbody id="eventbody">

                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-lg-12">
                <div class="panel panel-transparent">
                    <div class="panel-heading">逆变器数据对比</div>
                </div>

                <div class="panel-body">
                    <div class="tab-content">
                        <div id="power-tab-1" class="tab-pane">
                            <div id="dayPower" class="general-chart" style="height: 350px;width: 100%; border: 0px  ;">
                            </div>
                        </div>

                        <div id="power-tab-2" class="tab-pane">
                            <div id="monthPower" class="general-chart">

                            </div>
                        </div>
                        <div id="power-tab-3" class="tab-pane active">
                            <div id="yearPower" class="general-chart">

                            </div>
                        </div>
                    </div>

                </div>
            </div>
        </div>
</body>

</html>
