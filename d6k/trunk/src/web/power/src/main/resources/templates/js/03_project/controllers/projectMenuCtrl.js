/**
 * Created by reed on 2016/12/26.
 */
DaQOCloud.controller('projectMenuCtrl',['$scope','$stateParams','$state','projectMenuService','$rootScope',function ($scope, $stateParams, $state, projectMenuService,$rootScope) {
    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "resources/img/unselected.png"){
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

        $('#huabu div li').click(function() {
            if ($(this).hasClass("in")) {
                //$(this).next('ul').slideUp();
                //$(this).removeClass("nav_active");
                $(this).find('img').attr("src", "resources/templates/img/nav_tag02.png");
            } else {
                //$(this).next('ul').slideDown();
                //$(this).addClass("nav_active");
                $(this).find('img').attr("src", "resources/templates/img/nav_tag.png");
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

    $scope.goToPage = function(beakerId){
        if($rootScope.socket != undefined){
            $rootScope.socket.close();
        }
        $(".current_02").removeClass('current_02');
        $(event.currentTarget).addClass('current_02');
        $state.go('project.tabs.beakerTabs',{'breakerId':beakerId})
            .then(function () {
                $state.go('project.tabs.beakerTabs.run');
            });
    }

    $scope.goToProject = function(pjId,$event){
        if($rootScope.socket != undefined){
            $rootScope.socket.close();
        }
        $state.go('project.tabs',{'projectId':pjId})
            .then(function(){
                $state.go('project.tabs.run');
            });
        if($($event.currentTarget).next().hasClass('in')){
            $($event.currentTarget).find('img').attr("src", "resources/templates/img/nav_tag02.png");
        }else{
            $($event.currentTarget).find('img').attr("src", "resources/templates/img/nav_tag.png");
        }
        $(".current_02").removeClass('current_02');
    }

    $scope.collapseDevice = function ($event) {
        if($($event.currentTarget).next().hasClass('in')){
            $($event.currentTarget).find('img').attr("src", "resources/templates/img/nav_tag02.png");
        }else{
            $($event.currentTarget).find('img').attr("src", "resources/templates/img/nav_tag.png");
        }
    }
}]);
