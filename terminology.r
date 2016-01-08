#include <Carbon/Carbon.r>

#define Reserved8   reserved, reserved, reserved, reserved, reserved, reserved, reserved, reserved
#define Reserved12  Reserved8, reserved, reserved, reserved, reserved
#define Reserved13  Reserved12, reserved
#define dp_none__   noParams, "", directParamOptional, singleItem, notEnumerated, Reserved13
#define reply_none__   noReply, "", replyOptional, singleItem, notEnumerated, Reserved13
#define synonym_verb__ reply_none__, dp_none__, { }
#define plural__    "", {"", kAESpecialClassProperties, cType, "", reserved, singleItem, notEnumerated, readOnly, Reserved8, noApostrophe, notFeminine, notMasculine, plural}, {}

resource 'aete' (0, "MacMap Script Suite") {
	0x1,  // major version
	0x0,  // minor version
	english,
	roman,
	{
		"Standard Suite",
		"Common classes and commands for most applications.",
		'????',
		1,
		1,
		{
			/* Events */

			"open",
			"Open an object.",
			'aevt', 'odoc',
			reply_none__,
			'file',
			"The file to be opened.",
			directParamRequired,
			singleItem, notEnumerated, Reserved13,
			{

			},

			"quit",
			"Quit MacMap.",
			'aevt', 'quit',
			reply_none__,
			dp_none__,
			{

			},

			"close",
			"Close current document.",
			'core', 'clos',
			reply_none__,
			dp_none__,
			{

			}
		},
		{
			/* Classes */

		},
		{
			/* Comparisons */
		},
		{
			/* Enumerations */
		},

		"MacMap Suite",
		"",
		'????',
		1,
		1,
		{
			/* Events */

			"send",
			"Send command code to MacMap",
			'MapT', 'SenD',
			reply_none__,
			'type',
			"The command (four char code) to send to MacMap.",
			directParamRequired,
			singleItem, notEnumerated, Reserved13,
			{

			},

			"send with params",
			"Send command code to MacMap API with xml parameters",
			'MapT', 'SenP',
			reply_none__,
			'type',
			"The command (four char code) to send to MacMap.",
			directParamRequired,
			singleItem, notEnumerated, Reserved13,
			{
				"xmltree", 'pxml', 'TEXT',
				"Parameters to pass to function as XML tree.",
				optional,
				singleItem, notEnumerated, Reserved13
			}
		},
		{
			/* Classes */

		},
		{
			/* Comparisons */
		},
		{
			/* Enumerations */
		}
	}
};
