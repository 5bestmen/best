/**
 * Created by reed on 2017/1/22.
 */

DaQOCloud.controller('macManageCtrl',['$scope','$stateParams','$state','$filter','$modal','macManageService', function ($scope, $stateParams, $state,$filter,$modal,macManageService) {

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "img/unselected.png"){
                $('.unselected').attr("src","img/selected.png");
            }else{
                $('.unselected').attr("src","img/unselected.png");
            }
        });
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });

        var scrollTop = 0;
        var scrollLeft = 0;
        $('#huabu1').scroll(function () {

            if(scrollLeft < $(this).scrollLeft() || scrollLeft > $(this).scrollLeft()) {
                scrollLeft = $(this).scrollLeft();
                var dir = scrollLeft < $(this).scrollLeft() ? 1 : -1;
                $('#horizontal-ruler').scrollLeft(19 * dir + scrollLeft)
            }

            if(scrollTop < $(this).scrollTop() || scrollTop > $(this).scrollTop()) {
                scrollTop = $(this).scrollTop();
                var dir = scrollTop < $(this).scrollTop() ? 1 : -1;
                $('#vertical-ruler').scrollTop(19 * dir + scrollTop)

            }
        });
    });

    var modelData = {};

    //获取所有工程信息
    macManageService.getAllPjInfo().then(function (res) {
        $scope.proInfoList = angular.copy(res);
        modelData['proInfoList'] = angular.copy(res);
    })

    //获取所有节点信息
    macManageService.getAllUnitInfo().then(function (res) {
        $scope.unitInfoList = angular.copy(res);
        modelData['unitInfoList'] = angular.copy(res);
    })

    //获取所有设备种类信息
    macManageService.getDeviceKindInfo().then(function (res) {
        $scope.deviceKindInfoList = angular.copy(res);
        modelData['deviceKindInfoList'] = angular.copy(res);
    })

    //获取所有厂商信息
    macManageService.getVendorInfo().then(function (res) {
        $scope.deviceKindInfoList = angular.copy(res);
        modelData['vendorList'] = angular.copy(res);
    })

    //获取所有设备型号信息
    macManageService.getDeviceModel().then(function (res) {
        $scope.deviceKindInfoList = angular.copy(res);
        modelData['macModelList'] = angular.copy(res);
    })




    //获取所有设备信息
    $scope.initDeviceList = function () {
        macManageService.getAllDeviceInfoList().then(function (res) {
            $scope.deviceListSource = angular.copy(res);
            $scope.deviceInfoList = res;
            $scope.$apply($scope.deviceInfoList);
        })
    }

    $scope.initDeviceList();

    //搜索
    $scope.doFilter = function () {
        $scope.deviceInfoList = $filter('filter')($scope.deviceListSource,JSON.parse('{"'+$scope.ftKind+'":"'+$scope.ftContent+'"}'),false);
    }

    //显示全部
    $scope.cleanFilter = function () {
        $scope.ftKind = null;
        $scope.ftContent = null;
        $scope.deviceInfoList = $scope.deviceListSource;
    }

    //新建一个设备信息
    $scope.openMacInfoModal = function(){
        modelData['deviceInfo'] = {};
        modelData['deviceInfo'].strGuid = ($scope.deviceListSource.length + 1001).toString();
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/macInfoModal.html',
            controller : 'macInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy(modelData);
                }
            }
        });

        modalInstance.opened.then(function(){//模态窗口打开之后执行的函数
            console.log('macInfo modal is opened');
        });

        //模态窗口关闭之后接受返回值  执行函数
        modalInstance.result.then(function (result) {
            macManageService.insertDeviceInfo(result).then(function () {
                $scope.initDeviceList();
            })
        })
    }

    //修改一个设备信息
    $scope.openUpdateMacInfoModal = function(deviceInfo){
        modelData['deviceInfo'] = deviceInfo;
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/macInfoModal.html',
            controller : 'macInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy(modelData);
                }
            }
        });

        //模态窗口打开之后执行的函数
        modalInstance.opened.then(function(){
            console.log('macInfo modal is opened');
        });

        //模态窗口关闭之后接受返回值  执行函数
        modalInstance.result.then(function (result) {
            if(! angular.equals(modelData['deviceInfo'],result)){
                macManageService.updataDeviceInfo(result).then(function () {
                    $scope.initDeviceList();
                })
            }

        })
    }

    //删除一个设备的信息
    $scope.deleteDeviceInfo = function (deviceId) {
        macManageService.deleteDeviceInfo(deviceId).then(function () {
            $scope.initDeviceList();
        })
    }
}]);