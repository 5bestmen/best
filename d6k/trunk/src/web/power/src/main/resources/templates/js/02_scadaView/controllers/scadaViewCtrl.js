/**
 * Created by reed on 2017/1/9.
 */
DaQOCloud.controller('scadaViewCtrl',['$scope','$http','$filter','$state','$rootScope',function($scope, $http, $filter, $state, $rootScope) {

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight - 55);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
    }

    $scope.scada_view_scope = {
        scadaViewInfo : null//当前显示的Scada
    }
    $scope.Scadalist = null; 	//该工程下的Scada集合

    //scada视图所用类
    var gValue ={
        drawSettingsOperation : new DrawSettingsOperation(),
        elementOperationObj: new CElementOperation()
    };

    $scope.refleshData = function(id){
        return gValue.drawSettingsOperation.getScadaList(id).then(function(result){
            $scope.Scadalist = result;
            $scope.$apply($scope.Scadalist);
        })
    }

    //打开一个新的scada
    $scope.refleshScada = function(selectScadaId){
        gValue.drawSettingsOperation.stop();
        gValue.drawSettingsOperation.getOneScadaView(selectScadaId).then(function(result){
            $scope.scada_view_scope.scadaViewInfo = result;
            $("#scadaViewDiv").empty();
            var encode = LZString.decompressFromBase64($scope.scada_view_scope.scadaViewInfo.strContent)
            $("#scadaViewDiv").html(encode == null ? gValue.drawSettingsOperation.scadaViewInfo.strContent : encode);
            $("#mid").css("background-color",$("#tempInfo").css("background-color"))
            $("#tempInfo").css({"position":"absolute","left":"0px","right":"0px","top":"0px","bottom":"0px"});
            $("#tempInfo").children("[class*=skipRect]").each(function(i,ch){
                if($(ch).hasClass("skipRect")){
                    $(ch).css("border-width","0px");
                }
            })

            $("#tempInfo").children("[class*=myclock]").each(function(i,ch){
                $($(ch).find(".display")[0]).html("<div class='weekdays'></div><div class='ampm'></div><div class='digits'></div>")
                gValue.elementOperationObj.initClock($(ch));
            })
            // 此处   调用画图函数   重新画 Echars
            if($scope.scada_view_scope.scadaViewInfo.lstChartTendWidgetInfo.length >0 | $scope.scada_view_scope.scadaViewInfo.lstChartRealTimeWidgetInfo.length >0
                | $scope.scada_view_scope.scadaViewInfo.lstChartOneDataWidgetInfo.length >0 ){
                $scope.drawInitScadaEchars($scope.scada_view_scope.scadaViewInfo);
                gValue.drawSettingsOperation.start(selectScadaId);
            }

        })
    }

    //双击跳转事件监听
    $scope.skipto = function($event){
        if($($event.target)[0].tagName == "DIV"){
            if($($event.target).attr("class").split(" ")[0] == "skipRect"){
                var id = $filter('filter')($scope.scada_view_scope.scadaViewInfo.lstTransferWidgetInfo,{'strDivId':$($event.target).attr("id")})[0].strDesScadaId
                $scope.selectScadaId = id.toString();
                gValue.drawSettingsOperation.stop();
                $scope.refleshScada(id);
            }
        }
    }

    //选择工程弹出框确定按钮
    $scope.selectProject = function () {
        var selectPJ = $filter('filter')($scope.logininfo.pjID,{"intDBPkPjinfoid":Number($("input[name='project']:checked").val())})[0];
        if(!jQuery.isEmptyObject(selectPJ)){
            $scope.selectPj = angular.copy(selectPJ);
            $scope.refleshData($scope.selectPj.intDBPkPjinfoid).then(function(){
                $scope.openScada();
            });
            $scope.closeWindow();
        }else{
            alert("请选择一个工程");
        }
    }

    //选择SCADA弹出框确定按钮
    $scope.openScada = function(){
        var selectSD = $filter('filter')($scope.Scadalist,{"intScadaId":Number($("input[name='selectSd']:checked").val())})[0];
        if(!jQuery.isEmptyObject(selectSD)){
            $scope.refleshScada(selectSD.intScadaId);
            $scope.closeWindow();
        }else{
            alert("请选择一个WebScada图");
        }
    }
    $scope.state = true;
    $scope.hideTitle = function(){
        if($scope.state){
            $( ".part_r" ).hide("blind",{},200);
            $(".main_mid_02").stop().animate({height : $(".main_mid_02").height()+99+"px"},200);
            $(".part_content").stop().animate({top:"41px"},200);
            $("#tempInfo").stop().animate({top:"0px",buttom:"0px"},200);
            $scope.state = !$scope.state
        }else{
            $( ".part_r" ).show("blind",{},200);
            $(".main_mid_02").stop().animate({height : ($(".main_mid_02").height()-99)+"px"},200);
            $(".part_content").stop().animate({top:"150px"},200);
            $("#tempInfo").stop().animate({top:"0px"},200);
            $scope.state = !$scope.state
        }
    }

    $rootScope.$watch('$state.current.name',function (obj1,obj2) {
        if(obj1 !== obj2){
            gValue.drawSettingsOperation.stop();
        }
    })

    $(function() {
        $("#myDivDraw").css("height", document.documentElement.clientHeight - 205);
        $("#scadaViewDiv").css("height", document.documentElement.clientHeight - 205);
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });
        //var selectPJ = $filter('filter')($scope.logininfo.pjID,{"intDBPkPjinfoid":Number($("input[name='project']:checked").val())})[0];
        var selectPJ = []
        if( $scope.logininfo.pjID.length > 0){
            selectPJ = $scope.logininfo.pjID[0];
        }
        if(!jQuery.isEmptyObject(selectPJ)) {
            $scope.selectPj = angular.copy(selectPJ);
            $scope.refleshData($scope.selectPj.intDBPkPjinfoid).then(function(){
                $scope.$apply($scope.selectPj);
                $scope.openScada();
            });
        }

    });

}]);