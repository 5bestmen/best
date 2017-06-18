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

    <title>${sessionScope.APP_NAME}-实时数据</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/img/sun.png"/>" rel="shortcut icon"/>

    <link href="<c:url value="/resources/css/multiSelect/sol.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/alertify.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/multiSelect/bootstrap-multiselect.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sweetalert.min.css" />" rel="stylesheet">

    <style type="text/css">
        table.hovertable {
            font-family: verdana,arial,sans-serif;
            font-size:11px;
            color:#333333;
            border-width: 1px;
            border-color: #999999;
            border-collapse: collapse;
        }
        table.hovertable th {
            background-color:#c3dde0;
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #a9c6c9;
        }
        table.hovertable tr {
            background-color:#d4e3e5;
        }
        table.hovertable td {
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #a9c6c9;
        }

        .laydate-icon {
            background-image: url(/resources/img/Calendar.png);
        }

        .col-xs-2 {
            padding-left: 0px;
            width: 20%;
            padding-left: 20px;
        }

        .divcss4-right{width:25%; height:100%;float:right}

    </style>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>

<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script>


    // 加载日节电量曲线
    function loadCurveOfDay(paraID, name, nameY) {

        var realOption = {
            tooltip: {
                trigger: 'axis'
            },
            legend: {
                padding: 5,
                textStyle: {
                    color: 'white'
                },
                data: [name]
            },
            toolbox: {
                show: false,
                feature: {
                    magicType: {show: true, type: ['bar', 'line']},
                    restore: {show: true}
                }
            },
            xAxis: [
                {
                    //type: 'category',
                    boundaryGap: true,
                    axisLine: {onZero: true},
                    axisLabel: {
                        textStyle: {
                            color: 'white'
                        }
                    },
                    data: []
                }
            ],
            yAxis: [
                {
                    name: nameY,
                    nameTextStyle: {
                        color: 'white'
                    },
                    axisLabel: {
                        textStyle: {
                            color: 'white'
                        }
                    },
                    type: 'value'
                }
            ],
            series: [
                {
                    name: name,
                    type: 'line',
                    itemStyle: {
                        normal: {
                            color: '#32d2c9',
                            label: {show: false}
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

        var stationID = parent.getTreeStationID();
        var deviceID = parent.getTreeDeviceID();
        var level = parent.getTreeLevel();

        if (level != 4) {
            swal({
                title: "请先选择设备再查询！",
                text: "请先选择设备再查询！",
                confirmButtonText: "确定"
            });
            return;
        }

        var optionRealAttr = {
            ajax: true,
            URL: '/api/charts/stations/realCurve',
            robin: true,
            timeSpan: 60000, //日发电量1分钟更新一次
            getConfig: function () {
                return {s_id: stationID, d_id: deviceID, p_id: paraID};
            },
            formatNumber: true,
            EchartsOption: realOption,
            chartContainer: 'realCurve'
        };

        var realCurveChart = $.fn.GeneralChart.init(optionRealAttr);
        realCurveChart.load();
    }
</script>

<script>
    function getRadio(){
        var e = event.srcElement;
        if(e.type=="radio") {
            // 点击相同radio不处理
            if (e.value == defaultID) {
                return;
            }

            defaultID = e.value;
            var nameY = "Ia(A)";
            var name = "电流";
            var vv = e.value;
            switch (e.value) {
                case "1":
                    nameY = "Ia(A)";
                    name = "电流";
                    break;
                case "2":
                    nameY = "Ib(A)";
                    name = "电流";
                    break;
                case "3":
                    nameY = "Ic(A)";
                    name = "电流";
                    break;
                case "5":
                    nameY = "Ua(V)";
                    name = "电压";
                    break;
                case "6":
                    nameY = "Ub(V)";
                    name = "电压";
                    break;
                case "7":
                    nameY = "Uc(V)";
                    name = "电压";
                    break;
                case "9":
                    nameY = "Pa(W)";
                    name = "有功功率";
                    break;
                case "10":
                    nameY = "Pb(W)";
                    name = "有功功率";
                    break;
                case "11":
                    nameY = "Pc(W)";
                    name = "有功功率";
                    break;
                case "12":
                    nameY = "Qa(kVar)";
                    name = "无功功率";
                    break;
                case "13":
                    nameY = "Qb(kVar)";
                    name = "无功功率";
                    break;
                case "14":
                    nameY = "Qc(kVar)";
                    name = "无功功率";
                    break;
                case "15":
                    nameY = "CosΦA";
                    name = "功率因素";
                    break;
                case "16":
                    nameY = "CosΦB";
                    name = "功率因素";
                    break;
                case "17":
                    nameY = "CosΦC";
                    name = "功率因素";
                    break;

                default :
                    return;
            }

            loadCurveOfDay(e.value, name, nameY);
        }
    }
</script>

<script>

    var defaultID = 1;
    $(function () {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();

        loadDiRealData(true);
        loadAiRealData(false);

        // 默认显示第一个测点的值，A相电流
        loadCurveOfDay(1, "电流", "Ia(A)");
    });


    function refresh(){
        var level = parent.getTreeLevel();
        if (level != 4) {
            return;
        }

        loadDiRealData(false);
        loadAiRealData(false);
        //loadCalcRealData(false);
    }

    function loadAiRealData(check) {

        if (check) {
            var level = parent.getTreeLevel();
            if (level != 4) {
                swal({
                    title: "请先选择设备再查询！",
                    text: "请先选择设备再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }
        }

        var stationID = parent.getTreeStationID();
        var deviceID = parent.getTreeDeviceID();

        $.ajax({
            type : "POST",
            url: "/api/realtimeCharts/rdbDataAI",
            async : false,
            data : {
                "stationId": stationID,
                "deviceId": deviceID
            },
            success : function(result)
            {
                if (result.length < 17) {
                    swal({
                        title: "测量值配置有误！",
                        text: "测量值配置有误！",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                var tableObjI = $('#dataI tbody');
                var tableObjU = $('#dataU tbody');
                var tableObjP = $('#dataP tbody');
                var tableObjQ = $('#dataQ tbody');
                var tableObjCos = $('#dataCos tbody');
                tableObjI.empty();
                tableObjU.empty();
                tableObjP.empty();
                tableObjQ.empty();
                tableObjCos.empty();

                var trI = "<tr>";
                trI += "<th></th><th style=\"text-align:center\">描述</th><th style=\"text-align:center\">值</th><th style=\"text-align:center\">单位</th>";
                trI += "</tr>";
                trI += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trI += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"1\" name=\"curve\" checked=\"true\"/></th><td style=\"text-align:center; font-size:15px\">Ia</td>";
                trI += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[0].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trI += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[0].unit + "</td> </tr>";
                trI += "</tr>";
                trI += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trI += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"2\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Ib</td>";
                trI += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[1].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trI += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[1].unit + "</td> </tr>";
                trI += "</tr>";
                trI += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trI += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"3\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Ic</td>";
                trI += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[2].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trI += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[2].unit + "</td> </tr>";
                trI += "</tr>";

                tableObjI.append(trI);

                var trU = "<tr>";
                trU += "<th></th><th style=\"text-align:center\">描述</th><th style=\"text-align:center\">值</th><th style=\"text-align:center\">单位</th>";
                trU += "</tr>";
                trU += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trU += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"5\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Ua</td>";
                trU += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[4].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trU += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[4].unit + "</td> </tr>";
                trU += "</tr>";
                trU += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trU += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"6\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Ub</td>";
                trU += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[5].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trU += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[5].unit + "</td> </tr>";
                trU += "</tr>";
                trU += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trU += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"7\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Uc</td>r";
                trU += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[6].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trU += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[6].unit + "</td> </tr>";
                trU += "</tr>";

                tableObjU.append(trU);

                var trP = "<tr>";
                trP += "<th></th><th style=\"text-align:center\">描述</th><th style=\"text-align:center\">值</th><th style=\"text-align:center\">单位</th>";
                trP += "</tr>";
                trP += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trP += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"9\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Pa</td>";
                trP += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[8].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trP += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[8].unit + "</td> </tr>";
                trP += "</tr>";
                trP += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trP += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"10\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Pb</td>";
                trP += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[9].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trP += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[9].unit + "</td> </tr>";
                trP += "</tr>";
                trP += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trP += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"11\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Pc</td>";
                trP += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[10].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trP += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[10].unit + "</td> </tr>";
                trP += "</tr>";

                tableObjP.append(trP);

                var trQ = "<tr>";
                trQ += "<th></th><th style=\"text-align:center\">描述</th><th style=\"text-align:center\">值</th><th style=\"text-align:center\">单位</th>";
                trQ += "</tr>";
                trQ += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trQ += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"12\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Qa</td>";
                trQ += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[11].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trQ += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[11].unit + "</td> </tr>";
                trQ += "</tr>";
                trQ += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trQ += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"13\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Qb</td>";
                trQ += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[12].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trQ += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[12].unit + "</td> </tr>";
                trQ += "</tr>";
                trQ += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trQ += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"14\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">Qc</td>";
                trQ += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[13].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trQ += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[13].unit + "</td> </tr>";
                trQ += "</tr>";

                tableObjQ.append(trQ);


                var trCos = "<tr>";
                trCos += "<th></th><th style=\"text-align:center\">描述</th><th style=\"text-align:center\">值</th><th style=\"text-align:center\">单位</th>";
                trCos += "</tr>";
                trCos += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trCos += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"15\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">CosA</td>";
                trCos += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[14].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trCos += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[14].unit + "</td> </tr>";
                trCos += "</tr>";
                trCos += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trCos += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"16\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">CosB</td>";
                trCos += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[15].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trCos += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[15].unit + "</td> </tr>";
                trCos += "</tr>";
                trCos += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                trCos += "<th style=\"width:7%;height:20px;\"><input type=\"radio\" value=\"17\" name=\"curve\"/></th><td style=\"text-align:center; font-size:15px\">CosC</td>";
                trCos += "<td class=\"measurement\" data-measurementType=\"2\"" + " data-measurementID=" + result[16].code +" style=\"text-align:center; color:orange; font-size:15px\">"+"</td>";
                trCos += "<td style=\"text-align:center; color:#6c0; font-size:15px\">" + result[16].unit + "</td> </tr>";
                trCos += "</tr>";

                tableObjCos.append(trCos);

                var meaObject = $.fn.GeneralMeasurement.init();
                meaObject.load();

                if (result.length < 31) {
                    swal({
                        title: "<档位信息>配置有误！",
                        text: "<档位信息>配置有误！",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                //gearName = result[30].name;
                //gearCode = result[30].code;

                if (result.length < 40) {
                    swal({
                        title: "<开关次数>配置有误！",
                        text: "<开关次数>配置有误！",
                        confirmButtonText: "确定"
                    });

                    switchInit();

                    return;
                }

                var tableObjSwitch = $('#switch_info');
                tableObjSwitch.empty();

                var tr = "<div style=\"text-align: center\">";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[36].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[36].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[37].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[37].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[38].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[38].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[39].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[39].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";

                tableObjSwitch.append(tr);

                var meaObject = $.fn.GeneralMeasurement.init();
                meaObject.load();
            }
        });
    }

    function loadDiRealData(check) {

        if (check) {
            var level = parent.getTreeLevel();
            if (level != 4) {
                swal({
                    title: "请先选择设备再查询！",
                    text: "请先选择设备再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }
        }

        var stationID = parent.getTreeStationID();
        var deviceID = parent.getTreeDeviceID();

        $.ajax({
            type : "POST",
            url: "/api/realtimeCharts/rdbDataDI",
            async : false,
            data : {
                "stationId": stationID,
                "deviceId": deviceID
            },
            success : function(result)
            {
                if (result.length < 6) {
                    return;
                }

                var tableObjDI = $('#di_info');
                tableObjDI.empty();

                var tr = "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;float:left;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state stylel measurement red-bg\" data-measurementid=" + result[0].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[0].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tr += "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;float:left;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state style1 measurement red-bg\" data-measurementid=" + result[1].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[1].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tr += "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;float:left;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state style1 measurement red-bg\" data-measurementid=" + result[2].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[2].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tr += "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state style1 measurement red-bg\" data-measurementid=" + result[3].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[3].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tr += "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;float:left;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state style1 measurement red-bg\" data-measurementid=" + result[4].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[4].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tr += "<div class=\"col-md-3\">";
                tr += "<div style=\"clear:both;float:left;\" class=\"col-md-12\">";
                tr += "<div class=\"widget-state style1 measurement red-bg\" data-measurementid=" + result[5].code +" data-measurementtype=\"1\">";
                tr += "<div class=\"text-center\">";
                tr += "<h5 class=\"font-bold YX\">" + result[5].name + "</h5>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";
                tr += "</div>";

                tableObjDI.append(tr);

                var meaObject = $.fn.GeneralMeasurement.init();
                meaObject.load();

            }
        });
    }

    function loadCalcRealData(check) {

        if (check) {
            var level = parent.getTreeLevel();
            if (level != 4) {
                swal({
                    title: "请先选择设备再查询！",
                    text: "请先选择设备再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }
        }

        var stationID = parent.getTreeStationID();
        var deviceID = parent.getTreeDeviceID();

        $.ajax({
            type : "POST",
            url: "/api/realtimeCharts/rdbDataCalc",
            async : false,
            data : {
                "stationId": stationID,
                "deviceId": deviceID
            },
            success : function(result)
            {
                if (result.length < 4) {
                    return;
                }

                var tableObjCalc = $('#switch_info');
                tableObjCalc.empty();

                var tr = "<div style=\"text-align: center\">";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[0].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[0].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[1].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[1].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[2].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[2].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>" + result[3].name + " :</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result[3].code +" data-measurementType=\"3\"></span>";
                tr += "</div>";

                tableObjCalc.append(tr);

                var meaObject = $.fn.GeneralMeasurement.init();
                meaObject.load();
            }
        });
    }

    function switchInit() {
        var tableObjSwitch = $('#switch_info');
        tableObjSwitch.empty();

        var tr = "<div style=\"text-align: center\">";
        tr += "<span>负荷开关合闸次数:</span>";
        tr += "<span>0</span>";
        tr += "</div>";
        tr += "<div style=\"text-align: center\">";
        tr += "<span>调容开关升压次数:</span>";
        tr += "<span>0</span>";
        tr += "</div>";
        tr += "<div style=\"text-align: center\">";
        tr += "<span>调压开关合闸次数1:</span>";
        tr += "<span>0</span>";
        tr += "</div>";
        tr += "<div style=\"text-align: center\">";
        tr += "<span>调压开关合闸次数2:</span>";
        tr += "<span>0</span>";
        tr += "</div>";

        tableObjSwitch.append(tr);
    }
</script>

<script>
    Date.prototype.format = function (fmt) {
        var o = {
            "M+": this.getMonth() + 1, //月份
            "d+": this.getDate(), //日
            "h+": this.getHours(), //小时
            "m+": this.getMinutes(), //分
            "s+": this.getSeconds(), //秒
            "q+": Math.floor((this.getMonth() + 3) / 3), //季度
            "S": this.getMilliseconds() //毫秒
        };
        if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
        for (var k in o)
            if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
        return fmt;
    };

</script>

<div class="row" style="margin:20px;">
    <div class="row" style="margin:0 auto;float:left;width: 80%">
        <div class="row" id="di_info" style="float: left">
            <div class="col-md-3">
                <div style="clear:both;float:left;" class="col-md-12">
                    <div class="widget-state stylel red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">欠压报警</h5>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-4">
                <div style="clear:both;float:left;" class="col-md-12">
                    <div class="widget-state style1 red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">过压报警</h5>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-4">
                <div style="clear:both;float:left;" class="col-md-12">
                    <div class="widget-state style1 red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">油温报警</h5>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-4">
                <div style="clear:both;" class="col-md-12">
                    <div class="widget-state style1 red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">容量状态</h5>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-4">
                <div style="clear:both;float:left;" class="col-md-12">
                    <div class="widget-state style1 red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">当前档位</h5>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-4">
                <div style="clear:both;float:left;" class="col-md-12">
                    <div class="widget-state style1 red-bg">
                        <div class="text-center">
                            <h5 class="font-bold YX">通信建立</h5>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </div>

    <div id="switch_info" class="divcss4-right" style="float:left;width: 20%">
        <div style="text-align: center">
            <span>负荷开关合闸次数:</span>
            <span>0</span>
        </div>
        <div style="text-align: center">
            <span>调容开关升压次数:</span>
            <span>0</span>
        </div>
        <div style="text-align: center">
            <span>调压开关合闸次数1:</span>
            <span>0</span>
        </div>
        <div style="text-align: center">
            <span>调压开关合闸次数2:</span>
            <span>0</span>
        </div>
    </div>

    <div style="margin:20px 0">
        <div class="col-md-6 measurement-container" style="width: 100%">
            <div class="panel panel-transparent">
                <div class="panel-body">
                    <div id="realCurve" style="height: 350px ; width: 100%;"></div>
                </div>

            </div>
        </div>
    </div>
</div>

<div style="margin:30px 0;height:30%;" onclick="getRadio()">
    <table class="hovertable"; id="dataI"; style="float:left;height:30%;width: 19%";>
        <tr>
            <th></th><th>描述</th><th>值</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="1" name="curve" checked="true"/></th><td>Ia</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="2" name="curve"/></th><td>Ib</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="3" name="curve"/></th><td>Ic</td><td>-</td>
        </tr>
    </table>

    <table class="hovertable"; id="dataU"; style="float:left;height:30%;width: 19%;margin:0 10px">
        <tr>
            <th></th><th>描述</th><th>值</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="5" name="curve"/></th><td>Ua</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="6" name="curve"/></th><td>Ub</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="7" name="curve"/></th><td>Uc</td><td>-</td>
        </tr>
    </table>

    <table class="hovertable"; id="dataP"; style="float:left;height:30%;width: 19%">
        <tr>
            <th></th><th>描述</th><th>值</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="9" name="curve"/></th><td>Pa</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="10" name="curve"/></th><td>Pb</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="11" name="curve"/></th><td>Pc</td><td>-</td>
        </tr>
    </table>

    <table class="hovertable"; id="dataQ"; style="float:left;height:30%;width: 19%;margin:0 10px">
        <tr>
            <th></th><th>描述</th><th>值</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="12" name="curve"/></th><td>Qa</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="13" name="curve"/></th><td>Qb</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="14" name="curve"/></th><td>Qc</td><td>-</td>
        </tr>
    </table>

    <table class="hovertable"; id="dataCos"; style="float:left;height:30%;width: 19%">
        <tr>
            <th></th><th>描述</th><th>值</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="15" name="curve"/></th><td>CosA</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="16" name="curve"/></th><td>CosB</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <th style="width:7%;height:20px;"><input type="radio" value="17" name="curve"/></th><td>CosC</td><td>-</td>
        </tr>
    </table>
</div>

</body>

</html>
