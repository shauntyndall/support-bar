(function ($) {
    Drupal.behaviors.support_bar = {
        attach: function(context, settings) {
            $('body').append(Drupal.settings.support_bar.support_bar_html);
        }
    };
}(jQuery));