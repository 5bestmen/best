/**
 * Created by reed on 2017/1/22.
 */
DaQOCloud.controller('macInfoModalCtrl',['$scope','$http','$rootScope','$filter','$location','$modalInstance','paramObj','macManageService',function($scope, $http, $rootScope,$filter,$location,$modalInstance,paramObj,macManageService) {

    //模态框需要使用的参数
    $scope.proInfoList = paramObj['proInfoList'];
    $scope.unitInfoList = paramObj['unitInfoList'];
    $scope.deviceKindInfoList = paramObj['deviceKindInfoList'];
    $scope.vendorList = paramObj['vendorList'];
    $scope.macModelList = paramObj['macModelList'];
    $scope.deviceInfo = paramObj['deviceInfo'];

    if($scope.deviceInfo.intDeviceid != undefined){
        $scope.selectPj = $scope.deviceInfo.intDBPkPjinfoid + "";
        $scope.selectUnit = $scope.deviceInfo.intPjUnitId + "";
        $scope.selectKind = $scope.deviceInfo.intDeviceKindId + "";
        $scope.selectVendor = $scope.deviceInfo.intVendorId + "";
        $scope.selectMacModel = $scope.deviceInfo.intDeviceModelId + "";
    }

    //关闭模态框
    $scope.cancel = function(){
        $modalInstance.dismiss('cancel'); // 退出
    };

    $scope.changePjId = function () {
        $scope.selectUnit = null;
    }

    function changeStringToInt (obj) {
        if(obj == undefined || obj == null || obj == ""){
            return null;
        }else{
            return Number(obj);
        }
    }

    $scope.ok = function(){
        if($scope.selectPj == null || $scope.selectPj == undefined || $scope.selectPj == "0"){
            alert('请选择所属工程。');
            return;
        }
        if($scope.selectKind == null || $scope.selectKind == undefined || $scope.selectKind == "0"){
            alert('请选择设备种类信息。');
            return;
        }
        if($scope.selectVendor == null || $scope.selectVendor == undefined || $scope.selectVendor == "0"){
            alert('请选择设备厂商信息。');
            return;
        }
        if($scope.selectMacModel == null || $scope.selectMacModel == undefined || $scope.selectMacModel == "0"){
            alert('请选择设备型号信息。');
            return;
        }

        $scope.deviceInfo.intDBPkPjinfoid = changeStringToInt($scope.selectPj);
        $scope.deviceInfo.intPjUnitId = changeStringToInt($scope.selectUnit);
        $scope.deviceInfo.intDeviceKindId = changeStringToInt($scope.selectKind);
        $scope.deviceInfo.intVendorId = changeStringToInt($scope.selectVendor);
        $scope.deviceInfo.intDeviceModelId = changeStringToInt($scope.selectMacModel);
        $scope.deviceInfo.deleteFlag = 0;
        if($scope.deviceInfo.intPjUnitId == null){
            $scope.deviceInfo.deleteFlag = 1;
        }

        $modalInstance.close($scope.deviceInfo); //关闭并返回当前选项
    };

}]);