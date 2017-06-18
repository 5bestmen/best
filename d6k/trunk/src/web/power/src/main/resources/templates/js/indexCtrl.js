/**
 * Created by reed on 2016/12/30.
 */
DaQOCloud.controller('indexCtrl',['$scope','$stateParams','$state',function ($scope, $stateParams, $state) {
    //此js中可以放入全局变量（总$scope）或者公共方法
    console.info('index page');
    $scope.logininfo = {};
    $scope.nowDate = new Date().toLocaleDateString();

    //保存所有modal窗口id的数组
    $scope.windowsList=[];
    //窗口进栈
    $scope.openWindow=function(id) {
        if($scope.windowsList.length>0)
        {
            var lastId=$scope.windowsList[$scope.windowsList.length-1];
            if(lastId == id){
                // 防止双击打开同一个模态框两次导致灰屏
                return ;
            }
            $('#'+lastId).modal('hide');
        }
        $scope.windowsList.push(id);
        $('#'+id).modal({
            backdrop:"static",
            keyboard:false
        });
        $(".modal-backdrop").appendTo(document.body);
        var modalDialog =  $('#'+id).find(".modal-dialog").find(".modal-content");
        var minHeight = modalDialog.css("min-height");
        var minWidth = modalDialog.css("min-width");
        modalDialog.resizable({
            handles: 'ne, se, sw, nw',
            minHeight: minHeight,
            minWidth: minWidth,
        });

        modalDialog.draggable({
            handle: ".modal-header",
            stop: function(event, ui){
                if(ui.offset.left < 0){
                    ui.helper.offset({left : 0});
                }
                if(ui.offset.top < 0){
                    ui.helper.offset({top : 0});
                }
            }
        });
    };

    //窗口出栈
    $scope.closeWindow=function() {
        var id = $scope.windowsList.pop();
        $('#'+id).modal('hide');

        if($scope.windowsList.length>0)
        {
            var lastId=$scope.windowsList[$scope.windowsList.length-1];
            $('#'+lastId).modal({
                backdrop:"static",
                keyboard:false
            });
            $(".modal-backdrop").appendTo(document.body);
        }
    };

    //Echars重新绘图
    $scope.drawInitScadaEchars = function(scadaInfo){
        $(scadaInfo.lstChartTendWidgetInfo).each(function (n,ele){
            if($("#"+ele.strDivId)[0] != undefined){
                if(ele.strChartKind == "ecLine"){
                    chartcreater.creatEcharsLine(ele,$("#"+ele.strDivId));
                }
                if(ele.strChartKind == "ecArealine"){
                    chartcreater.creatEcharsAreaLine(ele,$("#"+ele.strDivId));
                }
            }
        });
        $(scadaInfo.lstChartOneDataWidgetInfo).each(function (n,ele){
            if($("#"+ele.strDivId)[0] != undefined){
                if(ele.strChartKind == "ecYibiaopan"){
                    chartcreater.creatYibiaopan(ele,$("#"+ele.strDivId));
                }
                if(ele.strChartKind == "dataText"){
                    $('#'+ ele.strDivId).find("span").val(ele.param.strDBValue);
                }
                if(ele.strChartKind == "switch"){
                    if(Number(ele.param.strDBValue)){
                        $('#'+ ele.strDivId).find("img").attr("src","resources/img/switch_on_img.png")
                    }else{
                        $('#'+ ele.strDivId).find("img").attr("src","resources/img/switch_off_img.png")
                    }
                }
                if(ele.strChartKind == "status"){
                    if(Number(ele.param.strDBValue)){
                        $('#'+ ele.strDivId).find("img").attr("src","resources/img/status_run.gif")
                    }else{
                        $('#'+ ele.strDivId).find("img").attr("src","resources/img/status_stop.png")
                    }
                }
            }
        });
        $(scadaInfo.lstChartRealTimeWidgetInfo).each(function (n,ele){
            if($("#"+ele.strDivId)[0] != undefined){
                if(ele.strChartKind == "ecBar"){
                    chartcreater.creatEcharsRainBowBar(ele,$("#"+ele.strDivId));
                }
                if(ele.strChartKind == "ecPie"){
                    chartcreater.creatEcharsPie(ele,$("#"+ele.strDivId));
                }
            }

        });
    }

    $scope.initPageView = function () {
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });

        var scrollTop = 0;
        var scrollLeft = 0;
        $('#huabu1').scroll(function () {

            if (scrollLeft < $(this).scrollLeft() || scrollLeft > $(this).scrollLeft()) {
                scrollLeft = $(this).scrollLeft();
                var dir = scrollLeft < $(this).scrollLeft() ? 1 : -1;
                $('#horizontal-ruler').scrollLeft(19 * dir + scrollLeft)
            }

            if (scrollTop < $(this).scrollTop() || scrollTop > $(this).scrollTop()) {
                scrollTop = $(this).scrollTop();
                var dir = scrollTop < $(this).scrollTop() ? 1 : -1;
                $('#vertical-ruler').scrollTop(19 * dir + scrollTop)

            }
        })

    }

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }
}]);