/**
 * Created by zyj on 2017/6/15.
 */


DaQOCloud.controller('ultraShortCurveCtrl',['$scope','$http','$rootScope','$filter','$location',function($scope, $http, $rootScope,$filter,$location) {

    // 基于准备好的dom，初始化echarts实例
    var ultraShortCurve = document.getElementById('ultraShortCurve');

    var ultraShortCurveChart = echarts.init(ultraShortCurve);

    function loadPieOfVolume(small, big) {

        // 指定图表的配置项和数据
        var option = {
            tooltip : {
                trigger: 'axis'
            },
            legend: {
                data:['实际值','预测值']
            },
            toolbox: {
                show : true,
                feature : {
                    mark : {show: true},
                    dataZoom : {show: true},
                    dataView : {show: true},
                    magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                    restore : {show: true},
                    saveAsImage : {show: true}
                }
            },
            calculable : true,
            dataZoom : {
                show : true,
                realtime : true,
                start : 20,
                end : 80
            },
            xAxis : [
                {
                    type : 'category',
                    boundaryGap : false,
                    data : function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push('2013-03-' + i);
                        }
                        return list;
                    }()
                }
            ],
            yAxis : [
                {
                    type : 'value'
                }
            ],
            series : [
                {
                    name:'实际值',
                    type:'line',
                    smooth:true,  //这句就是让曲线变平滑的
                    data:function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push(Math.round(Math.random()* 30));
                        }
                        return list;
                    }()
                },
                {
                    name:'预测值',
                    type:'line',
                    smooth:true,  //这句就是让曲线变平滑的
                    data:function (){
                        var list = [];
                        for (var i = 1; i <= 30; i++) {
                            list.push(Math.round(Math.random()* 10));
                        }
                        return list;
                    }()
                }
            ]
        };



        // 使用刚指定的配置项和数据显示图表。
        ultraShortCurveChart.setOption(option);
    }

    loadPieOfVolume(100,50);

    //浏览器大小改变时重置大小
    window.onresize = function () {
        ultraShortCurveChart.resize();
    };

}]);