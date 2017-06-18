/**
 * Created by lojoso on 2017/1/22.
 */

DaQOCloud.controller('proManageCtrl',['$scope','$stateParams','$state','userInfoMangeService','$filter','$modal','proManageService',function ($scope, $stateParams, $state,userInfoMangeService,$filter,$modal,proManageService) {

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


    $scope.initPjList = function () {
        proManageService.getAllProInfoList().then(function (res) {
            $scope.proInfoListSource = angular.copy(res);
            $scope.proInfoList = res;
            $scope.$apply($scope.proInfoList);
        })
    }

    $scope.initPjList();

    $scope.doFilter = function () {
        $scope.proInfoList = $filter('filter')($scope.proInfoListSource,JSON.parse('{"'+$scope.ftKind+'":"'+$scope.ftContent+'"}'),false);
    }

    $scope.cleanFilter = function () {
        $scope.ftKind = null;
        $scope.ftContent = null;
        $scope.proInfoList = $scope.proInfoListSource;
    }


    $scope.openPjInfoModal = function(){
        var newPjInfo = {'strPjName':null,'strType':null,'strOwnerAddress':null,'strContacts':null,'strContactWay':null,'strInfoNote':null};
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/pjInfoModal.html',
            controller : 'pjInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy(newPjInfo);
                }
            }
        });
        //模态窗口打开之后执行的函数
        modalInstance.opened.then(function(){
            console.log('modal is opened');
        });
        //模态窗口关闭之后接受返回值  执行函数
        modalInstance.result.then(function (result) {
            proManageService.insertProInfo(result).then(function () {
                $scope.initPjList();
            })
        })
    }

    $scope.openUpdatePjInfoModal = function(updatePjInfo){
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/pjInfoModal.html',
            controller : 'pjInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy(updatePjInfo);
                }
            }
        });
        //模态窗口打开之后执行的函数
        modalInstance.opened.then(function(){
            console.log('modal is opened');
        });
        //模态窗口关闭之后接受返回值  执行函数
        modalInstance.result.then(function (result) {
            proManageService.updataProInfo(result).then(function () {
                $scope.initPjList();
            })
        })
    }

    $scope.deletePjInfo = function (pjId) {
        proManageService.deleteProInfo(pjId).then(function () {
            $scope.initPjList();
        })
    }
}]);