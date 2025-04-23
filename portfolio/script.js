// Initialize AOS
AOS.init();

// Smooth Scroll
$(document).ready(function(){
    $('a[href^="#"]').on('click', function(event) {
        var target = this.hash;
        if (target) {
            event.preventDefault();
            $('html, body').animate({
                scrollTop: $(target).offset().top
            }, 800);
        }
    });
});
