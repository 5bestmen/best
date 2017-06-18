<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<html>
<head>
    <title>组串电流离散率</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="resources/css/bootstrap.min.css" rel="stylesheet">
    <link href="resources/css/metisMenu.min.css" rel="stylesheet">
    <link href="resources/css/sb-admin-2.css" rel="stylesheet">
    <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="resources/css/style.css" rel="stylesheet" type="text/css">

    <script src="resources/dist/js/html5shiv.min.js"></script>
    <script src="resources/dist/js/respond.min.js"></script>
</head>
<body>
    <script src="resources/dist/js/echarts/esl.js"></script>
    <script src="resources/dist/js/echarts/echarts.js"></script>
    <script src="resources/js/jquery.min.js"></script>
    <script src="resources/js/bootstrap.min.js"></script>
    <script src="resources/js/metisMenu.js"></script>
    <script src="resources/js/sb-admin-2.js"></script>
    <script src="resources/js/generalChart.js"></script>

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
                var myChart = ec.init(document.getElementById('stringCurrentDiscreteRate'));
                var stringChart = ec.init(document.getElementById('subCurrentDiscreteRate'));


                option = {
                    tooltip : {
                        trigger: 'axis',
                        axisPointer : {            // 坐标轴指示器，坐标轴触发有效
                            type : 'shadow'        // 默认为直线，可选为：'line' | 'shadow'
                        }
                    },
                    legend: {
                        textStyle:{
                            color:'white'
                        },
                        data:['阵列平均离散率']
                    },
                    toolbox: {
                        show : true,
                        feature : {
//                            mark : {show: true},
                            dataView : {show: true, readOnly: false},
                            magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
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
                            axisLabel : {
                                textStyle: {
                                    color:'white'
                                }
                            },
                            data : function (){
                                var list = [];
                                for (var i = 1; i <= 10; i++) {
                                    list.push("光伏阵列" + i);
                                }
                                return list;
                            }()
                        }
                    ],
                    series : [
                        {
                            name:'阵列平均离散率',
                            type:'bar',
                            smooth:true,
                            stack: '总量',
                            itemStyle : { normal: {label : {show: true, position: 'insideRight'}}},
                            data:function (){
                                var list = [];
                                for (var i = 1; i <= 10; i++) {
                                    list.push(Math.round(Math.random()* 5) + 12);
                                }
                                return list;
                            }()
                        }
                    ]
                };

                subOption = {
                    tooltip : {
                        trigger: 'axis'
                    },
                    legend: {
                        textStyle:{
                            color:'white'
                        },
                        data:['组串电流离散率']
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
                            axisLabel : {
                                textStyle: {
                                    color:'white'
                                }
                            },
                            data : function (){
                                var list = [];
                                for (var i = 1; i <= 5; i++) {
                                    list.push("组串" + i);
                                }
                                return list;
                            }()
                        }
                    ],
                    yAxis : [
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
                    series : [
                        {
                            name:'组串电流离散率',
                            type:'bar',
                            smooth:true,
                            itemStyle:{
                                normal:
                                {
                                    label : {show: true}
                                }
                            },
                            data:function (){
                                var list = [];
                                for (var i = 1; i <= 5; i++) {
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
                    var subTitle = "光伏阵列";
                    if (typeof param.seriesIndex != 'undefined') {
                        subTitle += (param.dataIndex + 1) + "离散系数前5的组串";
                    }
                    if (param.type == 'hover') {
                        document.getElementById('subTitle').innerHTML = subTitle;
                    }
                    else {
                        for(var i = 1; i <= 5; i++){
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
                <h3>组串电流离散率</h3>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <i class="fa fa-gears fa-fw"></i>组串电流离散率
                    </div>
                    <div class="panel-body">
                        <div id="stringCurrentDiscreteRate" class="general-chart"></div>
                    </div>
                </div>
            </div>
            <div class="col-lg-12">
                <div class="panel panel-info">
                    <div class="panel-heading"  id="subTitle">
                        <i class="fa fa-fw"></i>阵列离散系数前5的组串
                    </div>
                    <div class="panel-body">
                        <div id="subCurrentDiscreteRate" class="general-chart"></div>
                    </div>
                </div>
            </div>
        </div>
    <%--</div>
</div>--%>
</body>
</html>
