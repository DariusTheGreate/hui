#ifndef _ITERATOR_TAGS_
#define _ITERATOR_TAGS_

namespace hui{

	struct input_iterator_tag {};

	struct output_iterator_tag {};

	struct forward_iterator_tag : input_iterator_tag {};

	struct bidirectional_iterator_tag : forward_iterator_tag {};

	struct random_access_iterator_tag : bidirectional_iterator_tag {};

	struct contiguous_iterator_tag : random_access_iterator_tag {};
};

#endif

