/**
 * Created by reed on 2017/1/23.
 */

DaQOCloud.controller('unitManageCtrl',['$scope','$stateParams','$state','$filter','unitManageService','$modal',function ($scope, $stateParams, $state,$filter,unitManageService,$modal) {

    $scope.unitListManage = [];
    $scope.SourceunitListManage = [];
    unitManageService.getUnitInfo().then(function(result){
        $scope.unitListManage = angular.copy(result);
        $scope.SourceunitListManage = result;
        $scope.$apply($scope.unitListManage);
    })

    $scope.doFilter = function () {
        var kind = $("#filterKind").val();
        var content = $("#filterContent").val();
        $scope.unitListManage = $filter('filter')($scope.SourceunitListManage,JSON.parse('{"'+kind+'":"'+content+'"}'),false)
    }

    $scope.doClear = function(){
        $scope.unitListManage = $scope.SourceunitListManage;
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

    $scope.deleteUnit = function(id){
        unitManageService.deleteUnit(id).then(function () {
            unitManageService.getUnitInfo().then(function(result){
                $scope.unitListManage = angular.copy(result);
                $scope.SourceunitListManage = result;
                $scope.$apply($scope.unitListManage);
            })
        })
    }

    $scope.openUnitInfoModal = function(){
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/unitInfoModal.html',
            controller : 'unitInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy({});
                }
            }
        });

        modalInstance.result.then(function (result) {
            unitManageService.addUnit(result).then(function () {
                unitManageService.getUnitInfo().then(function(result){
                    $scope.unitListManage = angular.copy(result);
                    $scope.SourceunitListManage = result;
                    $scope.$apply($scope.unitListManage);
                })
            })
            console.log(result);
        },function (reason) {
            console.log(reason);
        });
    }

    $scope.editUnitInfoModal = function(unitid,pjid){
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/unitInfoModal.html',
            controller : 'unitInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy($filter('filter')($scope.SourceunitListManage,{intPjUnitId:unitid,intDBPkPjinfoid:pjid})[0]);
                }
            }
        });

        modalInstance.result.then(function (result) {
            unitManageService.undateUnit(result).then(function () {
                unitManageService.getUnitInfo().then(function(result){
                    $scope.unitListManage = angular.copy(result);
                    $scope.SourceunitListManage = result;
                    $scope.$apply($scope.unitListManage);
                })
            })
            console.log(result);
        },function (reason) {
            console.log(reason);
        });
    }
}]);