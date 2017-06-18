/**
 * Created by reed on 2017/1/11.
 */

DaQOCloud.controller('scadaMakeCtrl',['$scope','$filter','$modal','$stateParams',function ($scope, $filter, $modal, $stateParams) {

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 205;
        $("#left").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#hiddenDiv").css("height", mainHeight);
    }

    //scada绘制所用类
    var gValue ={
        elementOperationObj:new CElementOperation(),
        $selectDivOjb:null,
        $undoDivObj:[],
        eontrollGroupCollection:new ControllGroupCollection(),
        drawSettingsOperation : new DrawSettingsOperation(),
    };

    $scope.scada_make_scope = {
        svgList:null,           //svg文件列表
        selectImgObj : null,   //当前选择的图片对象
        imgList	: null,         //图片问价列表
        selectDivObject : null, //当前选中的DIV
        nowScadaViewInfo : null, //当前页面的Scada信息
        selectEcharsData : null, //需要编辑的实时数据绑定类
        echarDataShowList : {'lstParams' :[]},//属性列表中展示的数据点
    };

    function InitComponets(){
        $( "#drawtext2" ).spinner({min: 600,step: 50});
        $( "#drawtext3" ).spinner({min: 300,step: 10});
        $( "#edittext5" ).spinner({min: 0,max: 20,step: 1});
        $( "#edittext3" ).spinner({min: 10,step: 1});
        $( "#edittext7_1" ).spinner({min: 1,step: 1});
        $( "#edittext7_2" ).spinner({min: 1,step: 1});
        $( "#edittext9_1" ).spinner({min: 1,step: 1});
        $( "#edittext9_2" ).spinner({min: 1,step: 1});
        $( "#edittext8" ).spinner({min: 10,max:500,step: 5});
        $( "#edittext10" ).spinner({min: 1,max : 999,step: 1});
    }

    $scope.divmousedowndealfuc = function(elementid) {
        gValue.elementOperationObj.setCommandClick(elementid);
        console.log(elementid + "  divmousedowndealfuc");
        gValue.$selectDivOjb = null;
        gValue.eontrollGroupCollection.setSelectDivAndControlGroupShow(gValue.$selectDivOjb);
    };

    //线条点击事件
    $("#myDivDraw").on("mousedown","div",function(e){
        if (1==e.which){
            gValue.$selectDivOjb =  $(e.currentTarget);
            var classname =gValue.$selectDivOjb.attr("class");

            if (classname == "highcharts-container"|| classname == null || classname == undefined){
                gValue.$selectDivOjb = $(e.currentTarget.parentElement);
                classname = gValue.$selectDivOjb.attr("class");
            } else{
                gValue.$selectDivOjb =$(e.currentTarget);
            }

            if($.inArray(classname.split(" ")[0],["rect_rectClass","circle_rectClass","circleRect_rectClass","steep_rectClass","verticalLine_rectClass",
                    "textArea","imgArea","imgSVGArea","slashline","selfimg"])+1) {
                var temp = angular.copy($(e.currentTarget));
                temp.find(".ui-resizable-handle").remove();
                gValue.$undoDivObj.push(temp);
            }
            $scope.fontStyleText = gValue.$selectDivOjb.css('font-style');
            $scope.fontFamilyText = gValue.$selectDivOjb.css('font-family').replace(/"/g,'');
            $scope.scada_make_scope.selectDivObject = gValue.$selectDivOjb;
            $scope.getDataParams(gValue.$selectDivOjb.attr("class").split(" ")[0],gValue.$selectDivOjb.attr("id"));
            console.log(classname);
            gValue.eontrollGroupCollection.setSelectDivAndControlGroupShow(gValue.$selectDivOjb);
            //gValue.editGroupControl.setSelectDiv(gValue.$selectDivOjb);
            e.stopPropagation();
        }
    });

    //鼠标按下，计算起点，并开始执行绘图操作
    $("#myDivDraw").mousedown(function (e) {
        var startX = e.clientX - $("#left").width();
        var startY = e.clientY - $("#header").height() - $(".part_s").height() - 55;
        console.log(startX+":"+startY);
        gValue.elementOperationObj.setMouseDown(startX,startY);
        gValue.$selectDivOjb = null;
        gValue.eontrollGroupCollection.setSelectDivAndControlGroupShow(gValue.$selectDivOjb);
    });

    //鼠标移动，以最后一点确定绘制终点
    $("#myDivDraw").mousemove(function (e) {
        var endX = e.clientX - $("#left").width();
        var endY =e.clientY - $("#header").height() - $(".part_s").height() - 55;
        gValue.elementOperationObj.setMouseMove(endX,endY);
    });

    //鼠标抬起，绘制完成，确定终点，设置移动属性
    $("#myDivDraw").mouseup(function (e) {
        var endX = e.clientX - $("#left").width();
        var endY =e.clientY - $("#header").height() - $(".part_s").height() - 55;
        gValue.elementOperationObj.setMouseUp(endX,endY);
    });

    //删除当前元素
    $scope.deleteDiv = function(){
        gValue.eontrollGroupCollection.elementDeal.dealElement($scope.scada_make_scope.nowScadaViewInfo);
        gValue.$selectDivOjb = null;
        gValue.eontrollGroupCollection.setSelectDivAndControlGroupShow(gValue.$selectDivOjb);

    };

    //复制当前元素
    $scope.copyDiv = function(){
        var classname = gValue.$selectDivOjb.attr("class").toString().split(" ")[0];
        if($.inArray(classname,["rect_rectClass","circle_rectClass","circleRect_rectClass","steep_rectClass","verticalLine_rectClass",
                "textArea","imgArea","imgSVGArea","slashline","selfimg"])+1){
            if(gValue.$selectDivOjb.find(".ui-resizable-handle").length > 0){
                gValue.$selectDivOjb.resizable( "destroy" );
            }
            var copytar = angular.copy(gValue.$selectDivOjb);
            copytar.attr("id",classname+"_"+new Date().getTime());
            copytar.css("top",Number(copytar.css("top").replace("px",""))-10);
            copytar.css("left",Number(copytar.css("left").replace("px",""))+10);
            if(! (copytar.hasClass("steep_rectClass") | copytar.hasClass("verticalLine_rectClass") | copytar.hasClass("slashline"))) {
                copytar.resizable({ containment: "parent" });
            }
            copytar.draggable({ containment: "parent" });
            $("#myDivDraw").append(copytar);
            if(! (gValue.$selectDivOjb.hasClass("steep_rectClass") | gValue.$selectDivOjb.hasClass("verticalLine_rectClass") | gValue.$selectDivOjb.hasClass("slashline"))){
                gValue.$selectDivOjb.resizable({ containment: "parent" });
            }

        }
    };

    //置于最顶层
    $scope.moveUp = function () {
        gValue.eontrollGroupCollection.elementDeal.moveUpLayer();
    };

    //置于最底层
    $scope.moveDown = function () {
        gValue.eontrollGroupCollection.elementDeal.moveDownLayer();
    };

    /*--------------------------------  关于图关联的相关操作  ------------------------------------*/
    /*--------------------------------  关于SVG的相关操作  ------------------------------------*/
    $scope.changeImgType = function(type){
        $scope.scada_make_scope.imgList = angular.copy($filter('filter')(gValue.drawSettingsOperation.imgLib,{'kind':type})[0]);
        console.info($scope.scada_make_scope.imgList);
    };

    gValue.drawSettingsOperation.getPicNameFromLib().then(function () {
        $scope.changeImgType('gif');
        $scope.scada_make_scope.svgList = angular.copy(gValue.drawSettingsOperation.svgLib);
    });

    gValue.drawSettingsOperation.getPicNameByUser($scope.logininfo.userInfoID.intPkUserInfoId).then(function(){
        $scope.scada_make_scope.userImgList = $filter('filter')(gValue.drawSettingsOperation.imgLib,{'kind':"user"})[0];
        $scope.$apply($scope.scada_make_scope.userImgList);
        gValue.drawSettingsOperation.uploadImg($scope.logininfo.userInfoID.intPkUserInfoId,$scope.scada_make_scope.userImgList,
            function(target){
                $scope.$apply(target);
            });
    });

    /*--------------------------------  关于数据关联的相关操作  ------------------------------------*/
    $scope.selectProject = function () {
        var selectPJ = $filter('filter')($scope.logininfo.pjID,{"intDBPkPjinfoid":Number($("input[name='selectPj']:checked").val())})[0];
        if(!jQuery.isEmptyObject(selectPJ)){
            $scope.selectPj = angular.copy(selectPJ);
            $scope.refleshData($scope.selectPj.intDBPkPjinfoid);
            $scope.closeWindow();
        }else{
            alert("请选择一个工程");
        }
    };

    $scope.openScadaDiv = function(){
        var selectSD = $filter('filter')($scope.Scadalist,{"intScadaId":Number($("input[name='selectSd']:checked").val())})[0];
        if(!jQuery.isEmptyObject(selectSD)){
            $scope.selectSD = angular.copy(selectSD);
            $scope.openScada($scope.selectSD.intScadaId);
            $scope.closeWindow();
        }else{
            alert("请选择一个WebScada图");
        }
    };

    //获取Echars绑定的数据在集合中的下标
    $scope.getIndexFromDivId = function(lstData,divid) {

        var i=0;
        for(i=0;i<lstData.length;i++){

            if (lstData[i].strDivId==divid)
            {
                return i;
            }
        }
        return -1;
    };

    //打开Echars数据绑定模态框
    $scope.openEchars = function () {
        var divId =  gValue.$selectDivOjb.attr("id");
        var data;
        var index =-1;
        //时间段数据： 标准折线图   面积折线图
        if(gValue.$selectDivOjb.hasClass("ecLine")||gValue.$selectDivOjb.hasClass("ecArealine") ){
            index = $scope.getIndexFromDivId($scope.scada_make_scope.nowScadaViewInfo.lstChartTendWidgetInfo,divId);
            if(index==-1){
                data = new ChartTendWidgetInfo();
                data.strDivId = divId ;
                if(gValue.$selectDivOjb.hasClass("ecLine")){
                    data.strChartKind = 'ecLine';
                }else{
                    data.strChartKind = 'ecArealine';
                }
            }else{
                data = $scope.scada_make_scope.nowScadaViewInfo.lstChartTendWidgetInfo[index];
            }
            //为打开的模态框 传参
            var paramObj ={};
            paramObj["selectData"] = data;
            paramObj["lstDeviceInfo"] = $scope.circuitList;
            //打开模态框
            var modalInstance = $modal.open({
                templateUrl : 'resources/html/05_scadaMake/echars/echarsOperationModelDlg.html',
                controller : 'echarsOperationModelDlgCtrl',
                backdrop:false,
                resolve : {
                    paramObj : function(){
                        return angular.copy(paramObj);
                    }
                }
            });

            modalInstance.opened.then(function(){//模态窗口打开之后执行的函数
                console.log('modal is opened');
            });

            //模态框关闭返回参数的处理
            modalInstance.result.then(function (result) {

                console.log(result);
                $scope.scada_make_scope.echarDataShowList.lstParams = result.lstParams;
                //绘制图表
                if(gValue.$selectDivOjb.hasClass("ecLine")){
                    chartcreater.creatEcharsLine(result,gValue.$selectDivOjb);
                }else{
                    chartcreater.creatEcharsAreaLine(result,gValue.$selectDivOjb);
                }
                //更新绑定数据
                if(index==-1){
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartTendWidgetInfo.push(result);
                }
                else{
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartTendWidgetInfo[index] = result;
                }

            },function (reason) {
                console.log(reason);
            });

        }
        //实时数据：标准饼图  彩虹柱状图
        if(gValue.$selectDivOjb.hasClass("ecPie") || gValue.$selectDivOjb.hasClass("ecBar")){
            index = $scope.getIndexFromDivId($scope.scada_make_scope.nowScadaViewInfo.lstChartRealTimeWidgetInfo,divId);
            if(index==-1){
                data = new ChartRealTimeWidgetInfo();
                data.strDivId = divId ;
                if(gValue.$selectDivOjb.hasClass("ecPie")){
                    data.strChartKind = 'ecPie';
                }else{
                    data.strChartKind = 'ecBar';
                }

            }else{
                data = $scope.scada_make_scope.nowScadaViewInfo.lstChartRealTimeWidgetInfo[index];
            }

            var paramObj ={};
            paramObj["selectData"] = data;
            paramObj["lstDeviceInfo"] = $scope.circuitList;

            var modalInstance = $modal.open({
                templateUrl : 'resources/html/05_scadaMake/echars/echarsOperationModelRealTimelDlg.html',
                controller : 'echarsOperationModelRealTimeDlgCtrl',
                backdrop:false,
                resolve : {
                    paramObj : function(){
                        return angular.copy(paramObj);
                    }
                }
            });

            modalInstance.opened.then(function(){//模态窗口打开之后执行的函数
                console.log('modal is opened');
            });

            modalInstance.result.then(function (result) {
                $scope.scada_make_scope.echarDataShowList.lstParams = result.lstParams;
                console.log(result);
                //绘制图表
                if(gValue.$selectDivOjb.hasClass("ecPie")){
                    chartcreater.creatEcharsPie(result,gValue.$selectDivOjb);
                }else{
                    chartcreater.creatEcharsRainBowBar(result,gValue.$selectDivOjb);
                }
                //更新绑定数据
                if(index==-1){
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartRealTimeWidgetInfo.push(result);
                }
                else{
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartRealTimeWidgetInfo[index] = result;
                }

            },function (reason) {
                console.log(reason);
            });
        }

        //单点数据:仪表盘
        if(gValue.$selectDivOjb.hasClass("ecYibiaopan") || gValue.$selectDivOjb.hasClass("dataText") || gValue.$selectDivOjb.hasClass("switch")
            || gValue.$selectDivOjb.hasClass("status")){
            index = $scope.getIndexFromDivId($scope.scada_make_scope.nowScadaViewInfo.lstChartOneDataWidgetInfo,divId);
            if(index==-1){
                data = new CharOneDataWidgetInfo();
                data.strDivId = divId ;
                if(gValue.$selectDivOjb.hasClass("ecYibiaopan")){
                    data.strChartKind  = "ecYibiaopan";
                }else if(gValue.$selectDivOjb.hasClass("dataText")){
                    data.strChartKind  = "dataText";
                }else if(gValue.$selectDivOjb.hasClass("switch")){
                    data.strChartKind = "switch"
                }else if(gValue.$selectDivOjb.hasClass("status")){
                    data.strChartKind = "status"
                }
            }else{
                data = $scope.scada_make_scope.nowScadaViewInfo.lstChartOneDataWidgetInfo[index];
            }

            var paramObj ={};
            paramObj["selectData"] = data;
            paramObj["lstDeviceInfo"] = $scope.circuitList;

            var modalInstance = $modal.open({
                templateUrl : 'resources/html/05_scadaMake/echars/echarsOperationModelOneDatalDlg.html',
                controller : 'echarsOperationModelOneDataDlgCtrl',
                backdrop:false,
                resolve : {
                    paramObj : function(){
                        return angular.copy(paramObj);
                    }
                }
            });

            modalInstance.opened.then(function(){//模态窗口打开之后执行的函数
                console.log('modal is opened');
            });

            modalInstance.result.then(function (result) {
                $scope.scada_make_scope.echarDataShowList.lstParams = [];
                $scope.scada_make_scope.echarDataShowList.lstParams.push(result.param);
                console.log(result);
                //绘制图表
                if(gValue.$selectDivOjb.hasClass("ecYibiaopan")){
                    chartcreater.creatYibiaopan(result,gValue.$selectDivOjb);
                }else if(gValue.$selectDivOjb.hasClass("dataText")){
                    gValue.$selectDivOjb.find("span").text(result.param.strDBValue);
                }else if(gValue.$selectDivOjb.hasClass("switch")){
                    if(Number(result.param.strDBValue)){
                        gValue.$selectDivOjb.find("img").attr("src","resources/img/switch_on_img.png")
                    }else{
                        gValue.$selectDivOjb.find("img").attr("src","resources/img/switch_off_img.png")
                    }
                }else if(gValue.$selectDivOjb.hasClass("status")){
                    if(Number(result.param.strDBValue)){
                        gValue.$selectDivOjb.find("img").attr("src","resources/img/status_run.gif")
                    }else{
                        gValue.$selectDivOjb.find("img").attr("src","resources/img/status_stop.png")
                    }
                }
                //更新绑定数据
                if(index==-1){
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartOneDataWidgetInfo.push(result);
                }
                else{
                    $scope.scada_make_scope.nowScadaViewInfo.lstChartOneDataWidgetInfo[index] = result;
                }

            },function (reason) {
                console.log(reason);
            });
        }
    };

    //新建一个SCADA
    $scope.creatNewScada = function (){
        gValue.drawSettingsOperation.creatNewScada($scope.selectPj.intDBPkPjinfoid,$("#newScadaName").val()).then(function(result){
            $scope.closeWindow();
            $scope.refleshData($scope.selectPj.intDBPkPjinfoid).then(function (){
                $scope.selectScadaId = gValue.drawSettingsOperation.scadaViewInfo.intScadaId.toString();
                $scope.scada_make_scope.nowScadaViewInfo = gValue.drawSettingsOperation.scadaViewInfo;
                $("#modelScada input").each(function(i,e){
                    if($(e).val() == gValue.drawSettingsOperation.scadaViewInfo.intScadaId){
                        $(e).attr("checked","true");
                    }
                });
                gValue.drawSettingsOperation.scadaViewInfo.intScadaId;

                $scope.openScada(gValue.drawSettingsOperation.scadaViewInfo.intScadaId);
            });
        })
    };

    //根据工程ID获取SCADA集合
    $scope.refleshData = function(id){
        return gValue.drawSettingsOperation.getScadaList(id).then(function(result){

            $scope.Scadalist = result;
            $scope.$apply($scope.Scadalist);
        })
    };


    $scope.deleteScada = function(){
        if(null == gValue.drawSettingsOperation.scadaViewInfo ||　undefined == gValue.drawSettingsOperation.scadaViewInfo || "" == gValue.drawSettingsOperation.scadaViewInfo){
            $scope.closeWindow();
            alert("请先选择需要删除的SCADA图！");
        }else{
            gValue.drawSettingsOperation.deleteScada($scope.selectPj.intDBPkPjinfoid,gValue.drawSettingsOperation.scadaViewInfo.intScadaId).then(function(result){
                $scope.closeWindow();
                $scope.refleshData($scope.selectPj.intDBPkPjinfoid);
                $("#myDivDraw").html("");
                $( "#hiddenDiv" ).show( "blind", {}, 1000 )
            })
        }
    };

    $scope.openScada = function(id){
        //gValue.drawSettingsOperation.getPicNameFromLib();
        gValue.drawSettingsOperation.getOneScadaView(id).then(function(result){
            gValue.drawSettingsOperation.getPjAllDeviceInfo($scope.selectPj.intDBPkPjinfoid).then(function(re){
                $scope.circuitList = re;
            });
            $scope.scada_make_scope.nowScadaViewInfo = result;
            $("#myDivDraw").empty();
            $("#myDivDraw").css({
                'height':'675px',
                'width': '3000px',
                'background-color':'#ffffff'
            });
            var encode = LZString.decompressFromBase64(gValue.drawSettingsOperation.scadaViewInfo.strContent);
            $("#myDivDraw").html(encode == null ? gValue.drawSettingsOperation.scadaViewInfo.strContent : encode);
            //$("#myDivDraw").html(gValue.drawSettingsOperation.scadaViewInfo.strContent);

            if($("#myDivDraw").find("#tempInfo").length != 0){
                $("#myDivDraw").css({
                    'height':$("#tempInfo").height(),
                    'width': $("#tempInfo").width(),
                    'background-color':$("#tempInfo").css('background-color')
                });
                $("#myDivDraw").html($("#tempInfo").html());
            }
            $("#myDivDraw").children().each(function(n,child){
                if(! ($(child).hasClass("steep_rectClass") | $(child).hasClass("verticalLine_rectClass") | $(child).hasClass("slashline"))){
                    $(child).resizable({ containment: "parent" });
                }
                $(child).draggable({ containment: "parent" });
            });
            //此处  调用画图函数   重新画 Echars
            $scope.drawInitScadaEchars($scope.scada_make_scope.nowScadaViewInfo);
            $( "#hiddenDiv" ).hide( "blind", {}, 1000 );
            //$("#hiddenDiv").css("display","none");
        })
    };

    $scope.saveScada = function(){
        var newDiv = $("<div></div>");
        newDiv.attr("id","tempInfo");
        newDiv.css({
            "height":$("#myDivDraw").css("height"),
            "width":$("#myDivDraw").css("width"),
            "background-color":$("#myDivDraw").css("background-color"),
            "margin":"0px auto"
        });
        newDiv.html($("#myDivDraw").html());
        newDiv.find(".ui-resizable-handle").remove();
        $scope.scada_make_scope.nowScadaViewInfo.strStatus = "end";
        $scope.scada_make_scope.nowScadaViewInfo.strContent = LZString.compressToBase64(newDiv[0].outerHTML.toString());
        gValue.drawSettingsOperation.saveScada($scope.scada_make_scope.nowScadaViewInfo).then(function(result){
            gValue.drawSettingsOperation.scadaViewInfo.strContent = $("#mid").html();
            alert("保存成功");
        })
    };

    $scope.saveTransform = function(scadaid){
        var t = new TransferWidgetInfo();
        t.strDivId = gValue.eontrollGroupCollection.$selectDiv.attr("id");
        t.strDesScadaId = scadaid;
        var tempT =  $filter('filter')($scope.scada_make_scope.nowScadaViewInfo.lstTransferWidgetInfo,{'strDivId':t.strDivId});
        if(tempT.length > 0){
            tempT[0].strDesScadaId = scadaid;
        }else{
            $scope.scada_make_scope.nowScadaViewInfo.lstTransferWidgetInfo.push(t);
        }
        console.log($scope.scada_make_scope.nowScadaViewInfo)
    };

    $scope.skipto = function($event){
        if($($event.target)[0].tagName == "DIV"){
            if($($event.target).attr("class").split(" ")[0] == "skipRect"){
                var id = $filter('filter')($scope.scada_make_scope.nowScadaViewInfo.lstTransferWidgetInfo,{'strDivId':$($event.target).attr("id")})[0].strDesScadaId;
                $scope.selectScadaId = id.toString();
                $scope.openScada(id)
            }
        }
    };

    $scope.getSkipId = function(list,id){
        var Id;
        $(list).each(function(i,e){
            if(e.strDivId == id){
                Id = e.strDesScadaId;
            }
        });
        return Id;
    };

    $scope.getDataListParams = function(list,id){
        var lstParams;
        $(list).each(function(i,e){
            if(e.strDivId == id){
                lstParams = e.lstParams;
            }
        });
        return lstParams;
    };

    $scope.start = function(id){
        gValue.drawSettingsOperation.start(id);
    };

    $scope.stop = function(id){
        gValue.drawSettingsOperation.stop();
    };

    $scope.getDataOneParams = function(list,id){
        var param;
        $(list).each(function(i,e){
            if(e.strDivId == id){
                param = e.param;
            }
        });
        return param;
    };

    $scope.getDataParams = function(kind,id){
        $scope.scada_make_scope.echarDataShowList.lstParams = [];
        switch(kind){
            case "skipRect": $scope.TransoformScadaId = $scope.getSkipId($scope.scada_make_scope.nowScadaViewInfo.lstTransferWidgetInfo,id);break;
            case "ecLine":
            case "ecArealine":$scope.scada_make_scope.echarDataShowList.lstParams =$scope.getDataListParams($scope.scada_make_scope.nowScadaViewInfo.lstChartTendWidgetInfo,id);break;
            case "ecPie":
            case "ecBar":$scope.scada_make_scope.echarDataShowList.lstParams =$scope.getDataListParams($scope.scada_make_scope.nowScadaViewInfo.lstChartRealTimeWidgetInfo,id);break;
            case "ecYibiaopan":
            case "switch":
            case "status":
            case "dataText": $scope.scada_make_scope.echarDataShowList.lstParams.push($scope.getDataOneParams($scope.scada_make_scope.nowScadaViewInfo.lstChartOneDataWidgetInfo,id));break;
        }
        $scope.$apply($scope.scada_make_scope.echarDataShowList.lstParams);
    };

    $scope.textChange = function (){
        gValue.$selectDivOjb.text($scope.edittext);
    };
    $scope.fontFamilyChange = function (){
        gValue.$selectDivOjb.css("font-family",$scope.fontFamilyText);
    };
    $scope.fontStyleChange = function (){
        gValue.$selectDivOjb.css("font-style",$scope.fontStyleText);
    };

    $scope.echarResize = function (todo){
        var svalue;
        if(todo == 'add'){
            svalue = 105
        }else if(todo == 'sub'){
            svalue = 95
        }

        var width =  gValue.$selectDivOjb.width();
        width =Math.round(width * (Number(svalue)/100));
        width = width + 'px';
        gValue.$selectDivOjb.css("width",width);

        var height =  gValue.$selectDivOjb.height();
        height =Math.round(height * (Number(svalue)/100));
        height = height + 'px';
        gValue.$selectDivOjb.css("height",height);

        echarts.getInstanceByDom(gValue.$selectDivOjb[0]).resize();
    };

    $scope.undoFun = function () {
        if(gValue.$undoDivObj.length != 0){
            var untar = gValue.$undoDivObj[gValue.$undoDivObj.length - 1];
            if($("#myDivDraw").find("#"+untar.attr("id")).length == 0){
                $("#myDivDraw").append(untar);
                if(! ($("#"+untar.attr("id")).hasClass("steep_rectClass") | $("#"+untar.attr("id")).hasClass("verticalLine_rectClass") |
                    $("#"+untar.attr("id")).hasClass("slashline"))){
                    $("#"+untar.attr("id")).resizable({ containment: "parent" });
                }
                $("#"+untar.attr("id")).draggable({ containment: "parent" });
                gValue.$undoDivObj.pop();
                return;
            }
            $("#"+untar.attr("id")).css({
                "top":untar.css("top"),
                "left":untar.css("left"),
                "background-color":untar.css("background-color"),
                "color":untar.css("color"),
                "width":untar.css("width"),
                "height":untar.css("height"),
                "border-width":untar.css("border-width"),
                "border-color":untar.css("border-color")

            });
            gValue.$undoDivObj.pop();
        }
    };

    $scope.findInList = function(){
        var type = $("input[name='img']:checked").val();
        var source = $filter('filter')(gValue.drawSettingsOperation.imgLib,{'kind':type})[0];
        if($scope.findImg == ""){
            $scope.changeImgType(type);
            return;
        }
        $scope.scada_make_scope.imgList.imgList = angular.copy($filter('filter')(source.imgList,$scope.findImg,false));
    };
    $scope.findInSvgList = function(){
        if($scope.findSvg == ""){
            $scope.scada_make_scope.svgList = angular.copy(gValue.drawSettingsOperation.svgLib);
            return;
        }
        $scope.scada_make_scope.svgList = angular.copy($filter('filter')(gValue.drawSettingsOperation.svgLib,$scope.findSvg,false));
    };

    $scope.deleteUserImg = function(){
        var name = $("#selectUserImg").find(".ui-selected")[0];
        if(name != undefined){
            gValue.drawSettingsOperation.deleteUserImg($(name).text(),$scope.logininfo.userInfoID.intPkUserInfoId).then(function(){
                gValue.drawSettingsOperation.getPicNameByUser($scope.logininfo.userInfoID.intPkUserInfoId).then(function(res){
                    $scope.scada_make_scope.userImgList.imgList = res;
                    $("#userImgView").attr("src","");
                    $scope.$apply($scope.scada_make_scope.userImgList);
                })
            });
        }
    }

    $(function() {
        $("#myDivDraw").css("height", document.documentElement.clientHeight - 205);
        $("#scadaViewDiv").css("height", document.documentElement.clientHeight - 205);
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });
        //var selectPJ = $filter('filter')($scope.logininfo.pjID,{"intDBPkPjinfoid":Number($("input[name='selectPj']:checked").val())})[0];
        var selectPJ = $scope.logininfo.pjID[0];
        if(!jQuery.isEmptyObject(selectPJ)) {
            $scope.selectPj = angular.copy(selectPJ);
            $scope.refleshData($scope.selectPj.intDBPkPjinfoid);
            //$scope.$apply($scope.selectPj)
        }
        $("#huabu li[class!='active']").next('div.content').slideUp();

        $('#huabu li').click(function() {
            if($(this).hasClass("active")) {
                //$(this).next('div.content').slideUp();
                $(this).removeClass("active");
                $(this).find('.btn2 img').attr("src", "resources/img/up.png");
            } else {
                //$(this).next('div.content').slideDown();
                $(this).addClass("active");
                $(this).find('.btn2 img').attr("src", "resources/img/down.png");
            }

        });

        InitComponets();

        gValue.elementOperationObj.setTargetDiv($("#myDivDraw"));

        $("#selectImg").selectable({
            stop: function() {
                $( ".ui-selected", this ).each(function() {
                    $scope.scada_make_scope.selectImgObj = $(this).text();
                    var srcName = $scope.scada_make_scope.imgList.src + $scope.scada_make_scope.selectImgObj;
                    $("#imgView").attr("src",srcName);
                    $scope.scada_make_scope.selectDivObject.find("img").attr("src",srcName);
                    //$scope.$apply($scope.scada_make_scope.selectDivObject);
                    $("#nowImgSrcInput").val(srcName.split('/').pop());
                });
            }
        });

        $("#selectSvg").selectable({
            stop: function() {
                $( ".ui-selected", this ).each(function() {
                    var srcName = "resources/img/svgLib/"+$(this).text();
                    $("#svgView").attr("src",srcName);
                    $scope.scada_make_scope.selectDivObject.find("img").attr("src",srcName);
                });
            }
        });

        $("#selectUserImg").selectable({
            stop: function() {
                $( ".ui-selected", this ).each(function() {
                    var srcName = $scope.scada_make_scope.userImgList.src + $(this).text();
                    $("#userImgView").attr("src",srcName);
                    $scope.scada_make_scope.selectDivObject.find("img").attr("src",srcName);
                });
            }
        });
    });

}]);

/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////