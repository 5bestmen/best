<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<html>
<head>
    <title>逆变器功率离散率</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>

</head>
<body>
    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/echarts/esl.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/echarts/echarts.js"/>"></script>


<script>
    require.config({
        paths:{
            'echarts' : './js/echarts',
            'echarts/chart/bar' : './js/echarts',
            'echarts/chart/gauge' : './js/echarts',
            'echarts/chart/pie' : './js/echarts'
        }
    });

    require(
            [
                'echarts',						// 使用柱状图就加载bar模块，按需加载
                'echarts/chart/gauge',
                'echarts/chart/bar',
                'echarts/chart/pie'
            ],
            function (ec) {
                // 基于准备好的dom，初始化echarts图表
                var myChart = ec.init(document.getElementById('inverterPowerDiscreteRate'));
                var stringChart = ec.init(document.getElementById('inverterPowerCurve'));


                var option = {
                    tooltip : {
                        trigger: 'axis',
                        axisPointer : {            // 坐标轴指示器，坐标轴触发有效
                            type : 'shadow'        // 默认为直线，可选为：'line' | 'shadow'
                        }
                    },
                    legend: {
                        textStyle: {
                            color:'white'
                        },
                        data:['逆变器功率离散率']
                    },
                    toolbox: {
                        show : true,
                        feature : {
//                            mark : {show: true},
                            dataView : {show: true, readOnly: false},
                            magicType : {show: true, type: ['line', 'bar','stack', 'tiled']},
                            restore : {show: true},
                            saveAsImage : {show: true}
                        }
                    },
                    calculable : true,
                    xAxis : [
                        {
                            type : 'value',
                            axisLabel: {
                                formatter: '{value} %',
                                textStyle: {
                                    color:'white'
                                }
                            }
                        }
                    ],
                    yAxis : [
                        {
                            type : 'category',
                            axisLabel: {
                                textStyle: {
                                    color:'white'
                                }
                            },
                            data : function (){
                                var list = [];
                                for (var i = 1; i <= 6; i++) {
                                    list.push("逆变器" + i);
                                }
                                return list;
                            }()
                        }
                    ],
                    series : [
                        {
                            name:'逆变器功率离散率',
                            type:'bar',
                            smooth:true,
                            stack: '总量',
                            itemStyle : { normal: {label : {show: true, position: 'insideRight'}}},
                            data:function (){
                                var list = [];
                                for (var i = 1; i <= 6; i++) {
                                    list.push(Math.round(Math.random()* 5) + 8);
                                }
                                return list;
                            }()
                        }
                    ]
                };

                var subOption = {
                    tooltip : {
                        trigger: 'axis'
                    },
                    legend: {
                        textStyle:{
                            color:'white'
                        },
                        data:['逆变器功率']
                    },
                    toolbox: {
                        show : true,
                        feature : {
//                            mark : {show: true},
                            dataView : {readOnly:false},
                            magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                            restore : {show: true},
                            saveAsImage : {show: true}
                        }
                    },
                    calculable : true,
                    xAxis : [
                        {
                            type : 'category',
                            boundaryGap : true,
                            axisLabel: {
                                formatter: '{value} %',
                                textStyle: {
                                    color:'white'
                                }
                            },
                            data : function (){
                                var list = [];
                                for (var i = 1; i <= 24; i++) {
                                    list.push(i + "时");
                                }
                                return list;
                            }()
                        }
                    ],
                    yAxis : [
                        {
                            type : 'value',
                            axisLabel: {
                                formatter: '{value} MW',
                                textStyle: {
                                    color:'white'
                                }
                            }
                        }
                    ],
                    series : [
                        {
                            name:'逆变器功率',
                            type:'line',
                            smooth:true,
                            itemStyle:{
                                normal:
                                {
                                    label : {show: true}
                                }
                            },
                            data:function (){
                                var list = [];
                                for (var i = 1; i <= 24; i++) {
                                    list.push(Math.round(Math.random()* 5) + 12);
                                }
                                return list;
                            }(),
                            markPoint : {
                                data : [
                                    {type : 'max', name: '最大值'},
                                    {type : 'min', name: '最小值'}
                                ]
                            }
                        }
                    ]
                };

                var ecConfig = require('echarts/config');
                function eConsole(param) {
                    var subTitle = "";
                    if (typeof param.seriesIndex != 'undefined') {
                        subTitle += param.name + "功率曲线";
                    }
                    if (param.type == 'hover') {
                        document.getElementById('subTitle').innerHTML = subTitle;
                        for(var i = 1; i <= 24; i++){
                            subOption.series[0].data[i-1].value = Math.round(Math.random()* 5) + 12;
                        }
                    } else {
                        for(var i = 1; i <= 24; i++){
                            subOption.series[0].data[i-1].value = Math.round(Math.random()* 5) + 12;
                        }
                        document.getElementById('subTitle').innerHTML = subTitle;
                    }
                    console.log(param);
                    stringChart.setOption(subOption);
                }

                myChart.on(ecConfig.EVENT.CLICK, eConsole);

                // 为echarts对象加载数据
                myChart.setOption(option);
                stringChart.setOption(subOption);
            }
    );
</script>

<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>
    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h3 class="page-header">逆变器功率离散率</h3>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <i class="fa fa-gears fa-fw"></i>逆变器功率离散率
                    </div>
                    <div class="panel-body">
                        <div id="inverterPowerDiscreteRate" class="general-chart"></div>
                    </div>
                </div>
            </div>
            <div class="col-lg-12">
                <div class="panel panel-info">
                    <div class="panel-heading"  id="subTitle">
                        <i class="fa fa-fw"></i>逆变器功率曲线
                    </div>
                    <div class="panel-body">
                        <div id="inverterPowerCurve" class="general-chart"></div>
                    </div>
                </div>
            </div>
        </div>
    <%--</div>
</div>--%>
</body>
</html>
