/**
 * Created by reed on 2016/12/27.
 */

DaQOCloud.controller('projectCtrl',['$scope','$stateParams','$state','projectService','$filter',function ($scope, $stateParams, $state, projectService,$filter) {

    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "resources/templates/img/unselected.png"){
                $('.unselected').attr("src","resources/templates/img/selected.png");
            }else{
                $('.unselected').attr("src","resources/templates/img/unselected.png");
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

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

    // projectService.getPjInfoList(3).then(function (reuslt) {
    //     $scope.pjInfoList = reuslt;
    // }).then(function () {
    //     projectService.getDeviceKind().then(function (a) {
    //         $scope.deviceKindList = a;
    //     });
    // }).then(function () {
    //     projectService.getUnitInfo().then(function (b) {
    //         $scope.unitInfoList = b;
    //         console.info("projectPage");
    //     });
    // });

    $scope.getDeviceList = function(piid,unitid,kindid){
        var tPj = $filter('filter')($scope.pjInfoList,{intDBPkPjinfoid:piid})[0];
        return $filter('filter')(tPj.lstDeviceInfo,{intPjUnitId:unitid,intDeviceKindId:kindid})
    }

    $scope.getDeviceCount = function(piid,unitid){
        var tPj = $filter('filter')($scope.pjInfoList,{intDBPkPjinfoid:piid})[0];
        return $filter('filter')(tPj.lstDeviceInfo,{intPjUnitId:unitid});
    }
}]);